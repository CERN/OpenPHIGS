/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
*
*   Open PHIGS is free software: you can redistribute it and/or modify
*   it under the terms of the GNU Lesser General Public License as published by
*   the Free Software Foundation, either version 2.1 of the License, or
*   (at your option) any later version.
*
*   Open PHIGS is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Lesser General Public License for more details.
*
*   You should have received a copy of the GNU Lesser General Public License
*   along with Open PHIGS. If not, see <http://www.gnu.org/licenses/>.
******************************************************************************
* Changes:   Copyright (C) 2022-2023 CERN
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#ifdef GLEW
#include <GL/glew.h>
#include <GL/glx.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>

#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "util.h"
#include "private/wsxP.h"
#include "private/wsglP.h"
#include "private/wsbP.h"
#include "private/sofas3P.h"

short int wsgl_use_shaders = 1;
#define LOG_INT(DATA) \
   css_print_eltype(ELMT_HEAD(DATA)->elementType); \
   printf(":\tSIZE: %d\t", ELMT_HEAD(DATA)->length); \
   printf("CONTENT: %d\n", PHG_INT(DATA));

#define LOG_FLOAT(DATA) \
   css_print_eltype(ELMT_HEAD(DATA)->elementType); \
   printf(":\tSIZE: %d\t", ELMT_HEAD(DATA)->length); \
   printf("CONTENT: %f\n", PHG_FLOAT(DATA));

/*******************************************************************************
 * wsgl_init
 *
 * DESCR:       Initialize renderer
 * RETURNS:     Non zero or zero on error
 */

int wsgl_init(
              Ws *ws,
              Pgcolr *background,
              Pint select_size
              )
{
  int status;
  Wsgl_handle wsgl;
  wsgl = (Wsgl_handle) malloc(sizeof(Wsgl) + 4 * sizeof(GLuint) * select_size);
  if (wsgl == NULL) {
    return FALSE;
  }
  memset(wsgl, 0, sizeof(Wsgl));
  wsgl->struct_stack = stack_create(sizeof(Ws_struct), 10);
  if (wsgl->struct_stack == NULL) {
    free(wsgl);
    return FALSE;
  }
#ifdef DEBUG
  printf("wsgl_init: background color type %d (%f %f %f)\n",
         background->type,
         background->val.general.x,
         background->val.general.y,
         background->val.general.z
         );
#endif
  phg_nset_init(&wsgl->cur_struct.ast.asf_nameset,
                1,
                wsgl->cur_struct.ast.ast_buf);
  phg_nset_init(&wsgl->cur_struct.cur_nameset,
                WS_MAX_NAMES_IN_NAMESET / 32,
                wsgl->cur_struct.nameset_buf);
  phg_nset_init(&wsgl->cur_struct.lightstat,
                1,
                wsgl->cur_struct.lightstat_buf);
  memcpy(&wsgl->background, background, sizeof(Pgcolr));
  wsgl->render_mode = WS_RENDER_MODE_DRAW;
  wsgl->select_size = select_size;
  wsgl->select_buf  = (unsigned *) &wsgl[1];
  ws->render_context = wsgl;

  wsgl->dev_st.int_style     = -1;
  wsgl->dev_st.int_style_ind = -1;
  wsgl->dev_st.int_shad_meth = -1;
  /* initialise shaders */
  wsgl_clear_geometry();
  wsgl_shaders(ws);
  status = TRUE;

  return status;
}

/*******************************************************************************
 * wsgl_close
 *
 * DESCR:       Close
 * RETURNS:     N/A
 */

void wsgl_close(
                Ws *ws
                )
{
  Wsgl_handle wsgl = ws->render_context;

  free(wsgl->struct_stack);
  free(ws->render_context);
}

/*******************************************************************************
 * wsgl_set_window
 *
 * DESCR:       Set render window coordinates
 * RETURNS:     N/A
 */
void wsgl_set_window(
                     Ws *ws,
                     Plimit3 *win
                     )
{
  Wsgl_handle wsgl = ws->render_context;

  wsgl->win_changed = 1;
  memcpy(&wsgl->cur_win, win, sizeof(Plimit3));
}

/*******************************************************************************
 * wsgl_set_viewport
 *
 * DESCR:       Set render window viewport
 * RETURNS:     N/A
 */
void wsgl_set_viewport(
                       Ws *ws,
                       Plimit3 *vp
                       )
{
  Wsgl_handle wsgl = ws->render_context;

  wsgl->vp_changed = 1;
#ifdef DEBUG
  printf("setting vp (%f %f) (%f %f) (%f %f)\n", vp->x_min, vp->x_max,  vp->y_min, vp->y_max,  vp->z_min, vp->z_max);
#endif
  memcpy(&wsgl->cur_vp, vp, sizeof(Plimit3));
}

/*******************************************************************************
 * wsgl_set_hlhsr_mode
 *
 * DESCR:       Set render depth mode
 * RETURNS:     N/A
 */
void wsgl_set_hlhsr_mode(
                         Ws *ws,
                         Pint hlhsr_mode
                         )
{
  Wsgl_handle wsgl = ws->render_context;

  wsgl->hlhsr_mode = hlhsr_mode;
  wsgl->hlhsr_changed = 1;
}

/*******************************************************************************
 * wsgl_clear
 *
 * DESCR:       Clear render window
 * RETURNS:     N/A
 */
void wsgl_clear(
                Ws *ws
                )
{
#ifdef DEBUG
  printf("wsgl_clear\n");
#endif
  Phg_ret ret;
  Pgcolr gcolr;
  Wsgl_handle wsgl = ws->render_context;
  char buffer[6];
  char * xdg_session_type;
  /* Try to get the background color from the color table entry for this ws */
  phg_wsb_inq_LUT_entry(ws, 0, PINQ_REALIZED, PHG_ARGS_COREP, &ret, &gcolr, NULL);
  if (ret.err == 0) {
    wsgl->background.val.general.x = gcolr.val.general.x;
    wsgl->background.val.general.y = gcolr.val.general.y;
    wsgl->background.val.general.z = gcolr.val.general.z;
  } else {
    wsgl->background.val.general.x = 0.;
    wsgl->background.val.general.y = 0.;
    wsgl->background.val.general.z = 0.;
#ifdef DEBUG
    printf("INFO: background color index 0 not defined. Using default black.\n");
#endif
  }
#ifdef DEBUG
  printf("wsgl_setup_background: Setting background to (%f %f %f)\n",  gcolr.val.general.x,gcolr.val.general.y,gcolr.val.general.z);
#endif
  if (ws->drawable_id != 0){
    glXMakeContextCurrent(ws->display, ws->drawable_id, ws->drawable_id, ws->glx_context);
  }
  wsgl_clear_geometry();
  xdg_session_type = getenv("XDG_SESSION_TYPE");
  if (xdg_session_type){
    strncpy(buffer, xdg_session_type, 5);
  } else {
    strncpy(buffer, "unkwn", 5);
  }
  if (ws->has_double_buffer && ( 0==strncmp(buffer, "x11", 3) || (0==strncmp(buffer, "tty", 3)))) {
#ifdef DEBUG
    printf("Swapping buffers in clear\n");
#endif
    glXSwapBuffers(ws->display, ws->drawable_id);
  }
  else {
#ifdef DEBUG
    printf("Clear: flush buffers\n");
#endif
    glFlush();
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*******************************************************************************
 * wsgl_flush
 *
 * DESCR:       Flush settings to render window
 * RETURNS:     N/A
 */
void wsgl_flush(
                Ws *ws
                )
{
  Ws_xform ws_xform;
  GLint x, y;
  GLsizei w, h;
  int clear_flag = 0;
  Wsgl_handle wsgl = ws->render_context;

  if (ws->drawable_id != 0){
    glXMakeContextCurrent(ws->display, ws->drawable_id, ws->drawable_id, ws->glx_context);
  }
  if (wsgl->vp_changed || wsgl->win_changed) {
    phg_wsx_compute_ws_transform(&wsgl->cur_win, &wsgl->cur_vp, &ws_xform);
    x = (GLint)   (ws_xform.offset.x - ws_xform.scale.x);
    y = (GLint)   (ws_xform.offset.y - ws_xform.scale.y);
    w = (GLsizei) (ws_xform.scale.x * 2.0);
    h = (GLsizei) (ws_xform.scale.y * 2.0);

#ifdef DEBUG
    printf("View: %f %f %f\n"
           "      %f %f %f\n",
           ws_xform.offset.x,
           ws_xform.offset.y,
           ws_xform.offset.z,
           ws_xform.scale.x,
           ws_xform.scale.y,
           ws_xform.scale.z);
    printf("%d %d %d %d\n", x, y, w, h);
#endif

    glViewport(x, y, w, h);
    glDepthRange(ws_xform.scale.z, ws_xform.offset.z);

    if (wsgl->vp_changed) {
      wsgl->vp_changed = 0;

#ifndef REMOVE
      ws->ws_rect.x      = (int) wsgl->cur_vp.x_min;
      ws->ws_rect.y      = (int) wsgl->cur_vp.y_min;
      ws->ws_rect.width  = (int) wsgl->cur_vp.x_max - wsgl->cur_vp.x_min;
      ws->ws_rect.height = (int) wsgl->cur_vp.y_max - wsgl->cur_vp.y_min;

#ifdef DEBUG
      printf("Viewport changed: %d x %d\n",
             ws->ws_rect.width,
             ws->ws_rect.height);
#endif

      XResizeWindow(ws->display,
                    ws->drawable_id,
                    wsgl->cur_vp.x_max,
                    wsgl->cur_vp.y_max);
#endif /* REMOVE */
    }

    if (wsgl->win_changed) {
      wsgl->win_changed = 0;
    }

    clear_flag = 1;
  }

  if (wsgl->hlhsr_changed) {
    if (wsgl->hlhsr_mode == PHIGS_HLHSR_MODE_ZBUFF) {
#ifdef DEBUG
      printf("Enable z-buffer\n");
#endif
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glAlphaFunc (GL_GREATER, 0.1);
      glEnable(GL_ALPHA_TEST);
    }
    else if (wsgl->hlhsr_mode == PHIGS_HLHSR_MODE_NONE) {
#ifdef DEBUG
      printf("Disable z-buffer\n");
#endif
      glDisable(GL_DEPTH_TEST);
    }
    wsgl->hlhsr_changed = 0;
  }

  glClearColor(wsgl->background.val.general.x,
               wsgl->background.val.general.y,
               wsgl->background.val.general.z,
               0.0);
#ifdef DEBUG
  printf("wsgl: clear to background color %f %f %f\n",
         wsgl->background.val.general.x,
         wsgl->background.val.general.y,
         wsgl->background.val.general.z
         );
#endif
  if (clear_flag) {
    wsgl_clear(ws);
  }
}

/*******************************************************************************
 * init_rendering_state
 *
 * DESCR:       Initialize rendering state helper function
 * RETURNS:     N/A
 */
static void init_rendering_state(
                                 Ws *ws
                                 )
{
  Wsgl_handle wsgl = ws->render_context;

  wsgl->cur_struct.hlhsr_id = PHIGS_HLHSR_ID_OFF;
  wsgl_update_hlhsr_id(ws);
  phg_mat_identity(wsgl->composite_tran);
  phg_mat_identity(wsgl->cur_struct.global_tran);
  phg_mat_identity(wsgl->cur_struct.local_tran);
  wsgl_set_line_ind(ws, &wsgl->cur_struct.ast.bundl_group, 0);
  wsgl_set_line_ind(ws, &wsgl->cur_struct.ast.indiv_group, 0);
  wsgl_set_marker_ind(ws, &wsgl->cur_struct.ast.bundl_group, 0);
  wsgl_set_marker_ind(ws, &wsgl->cur_struct.ast.indiv_group, 0);
  wsgl_set_text_ind(ws, &wsgl->cur_struct.ast.bundl_group, 0);
  wsgl_set_text_ind(ws, &wsgl->cur_struct.ast.indiv_group, 0);
  wsgl->cur_struct.ast.char_ht = 0.01;
  wsgl->cur_struct.ast.anno_char_ht = 0.01;
  wsgl->cur_struct.ast.text_path = PPATH_RIGHT;
  wsgl->cur_struct.ast.anno_text_path = PPATH_RIGHT;
  wsgl->cur_struct.ast.text_align.hor = PHOR_NORM;
  wsgl->cur_struct.ast.text_align.vert = PVERT_NORM;
  wsgl->cur_struct.ast.anno_text_align.hor = PHOR_NORM;
  wsgl->cur_struct.ast.anno_text_align.vert = PVERT_NORM;
  wsgl->cur_struct.ast.char_up_vec.delta_x = 0.0;
  wsgl->cur_struct.ast.char_up_vec.delta_y = 1.0;
  wsgl->cur_struct.ast.anno_char_up_vec.delta_x = 0.0;
  wsgl->cur_struct.ast.anno_char_up_vec.delta_y = 1.0;
  wsgl->cur_struct.ast.disting_mode = PDISTING_YES;
  wsgl->cur_struct.ast.cull_mode = PCULL_NONE;
  wsgl_set_edge_ind(ws, &wsgl->cur_struct.ast.bundl_group, 0);
  wsgl_set_edge_ind(ws, &wsgl->cur_struct.ast.indiv_group, 0);
  wsgl_set_int_ind(ws, &wsgl->cur_struct.ast.bundl_group, 0);
  wsgl_set_int_ind(ws, &wsgl->cur_struct.ast.indiv_group, 0);
  phg_nset_names_set_all(&wsgl->cur_struct.ast.asf_nameset);
  wsgl_set_view_ind(ws, 0);
  wsgl_set_clip_ind(ws, 0);
  phg_nset_names_clear_all(&wsgl->cur_struct.cur_nameset);
  phg_nset_names_clear_all(&wsgl->cur_struct.lightstat);
  wsgl->cur_struct.pick_id = 0;
}

/*******************************************************************************
 * wsgl_begin_rendering
 *
 * DESCR:       Start a rendiering session for workstation
 * RETURNS:     N/A
 */
void wsgl_begin_rendering(
                          Ws *ws
                          )
{
#ifdef DEBUG
  printf("Begin rendering\n");
#endif
  if (wsgl_use_shaders){
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (currentProgram != ws->program){
#ifdef DEBUG
      printf("Switching back to program %d\n", ws->program);
#endif
      glUseProgram(ws->program);
    }
  }
  if (ws->drawable_id != 0){
    glXMakeContextCurrent(ws->display, ws->drawable_id, ws->drawable_id, ws->glx_context);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  init_rendering_state(ws);
}

/*******************************************************************************
 * wsgl_end_rendering
 *
 * DESCR:       End a rendiering session
 * RETURNS:     N/A
 */
void wsgl_end_rendering(
                        Ws *ws
                        )
{
#ifdef DEBUG
  printf("End rendering\n");
#endif

  if (ws->has_double_buffer) {
#ifdef DEBUG
    printf("Swapping buffers end rendering\n");
#endif
    glXSwapBuffers(ws->display, ws->drawable_id);
  }
  else {
#ifdef DEBUG
    printf("Flushing buffers\n");
#endif
    glFlush();
  }
}

/*******************************************************************************
 * store_cur_struct
 *
 * DESCR:       Store current structure information helper function
 * RETURNS:     N/A
 */

static void store_cur_struct(
                             Ws *ws
                             )
{
  uint32_t encode;
  Wsgl_handle wsgl = ws->render_context;

  if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
#ifdef DEBUGINP
    printf("\tOffset: %d, Pick ID: %d\n",
           wsgl->cur_struct.offset,
           wsgl->cur_struct.pick_id);
#endif
    encode = (wsgl->cur_struct.offset << 16) | wsgl->cur_struct.pick_id;
    glLoadName(encode);
  }
}

/*******************************************************************************
 * update_cur_struct
 *
 * DESCR:       Update current structure information helper function
 * RETURNS:     N/A
 */
static void update_cur_struct(
                              Ws *ws
                              )
{
  Wsgl_handle wsgl = ws->render_context;

  store_cur_struct(ws);
  wsgl->cur_struct.offset++;
}

static int check_draw_primitive(
                                Ws *ws
                                )
{
  int status;
  Wsgl_handle wsgl = ws->render_context;

  switch (wsgl->render_mode) {
  case WS_RENDER_MODE_DRAW:
    if (wsgl->invis_filter.used) {
      if (!phg_nset_names_intersect(&wsgl->cur_struct.cur_nameset,
                                    wsgl->invis_filter.incl) ||
          phg_nset_names_intersect(&wsgl->cur_struct.cur_nameset,
                                   wsgl->invis_filter.excl)) {
        status = TRUE;
      }
      else {
        status = FALSE;
      }
    }
    else {
      status = TRUE;
    }
    break;

  case WS_RENDER_MODE_SELECT:
    if (wsgl->pick_filter.used) {
      if (phg_nset_names_intersect(&wsgl->cur_struct.cur_nameset,
                                   wsgl->pick_filter.incl) &&
          !phg_nset_names_intersect(&wsgl->cur_struct.cur_nameset,
                                    wsgl->pick_filter.excl)) {
        status = TRUE;
      }
      else {
        status = FALSE;
      }
    }
    else {
      status = TRUE;
    }
    break;

  default:
    status = TRUE;
    break;
  }

  return status;
}

/*******************************************************************************
 * check_highlight_primitive
 *
 * DESCR:       check if the current primitive is highlighted
 * RETURNS:     N/A
 */
static int check_highlight_primitive(
                                     Ws *ws
                                     )
{
  int status;
  Wsgl_handle wsgl = ws->render_context;

  if (wsgl->highl_filter.used) {
    if (!phg_nset_names_intersect(&wsgl->cur_struct.cur_nameset,
                                  wsgl->highl_filter.incl) ||
        phg_nset_names_intersect(&wsgl->cur_struct.cur_nameset,
                                 wsgl->highl_filter.excl)) {
      status = FALSE;
    }
    else {
      status = TRUE;
    }
  }
  else {
    status = FALSE;
  }
  return status;
}

/*******************************************************************************
 * wsgl_begin_structure
 *
 * DESCR:       Mark the beginning of a new structure element
 * RETURNS:     N/A
 */
void wsgl_begin_structure(
                          Ws *ws,
                          Pint struct_id
                          )
{
  Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
  printf("Begin new structure element: %d\n", struct_id);
  printf("Old was: %d\n", wsgl->cur_struct.id);
  printf("Push: offset = %d\n",
         wsgl->cur_struct.offset);
#endif

  stack_push(wsgl->struct_stack, (caddr_t) &wsgl->cur_struct);
  wsgl->cur_struct.id      = struct_id;
  wsgl->cur_struct.offset  = 0;
  phg_mat_copy(wsgl->cur_struct.global_tran, wsgl->composite_tran);
  phg_mat_identity(wsgl->cur_struct.local_tran);
  wsgl_set_clip_ind(ws, 0);
  wsgl_set_alpha_channel(ws, 1.0);
  wsgl_update_modelview(ws);

  if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
#ifdef DEBUG
    printf("\tPush name: %d\n", struct_id);
#endif
    glPushName(struct_id);
    glPushName(-1);
    store_cur_struct(ws);
  }
}

/*******************************************************************************
 * wsgl_end_structure
 *
 * DESCR:       Mark the ending of a structure element
 * RETURNS:     N/A
 */
void wsgl_end_structure(
                        Ws *ws
                        )
{
  Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
  printf("End structure element: %d\n", wsgl->cur_struct.id);
#endif

   stack_pop(wsgl->struct_stack, (caddr_t) &wsgl->cur_struct);
   wsgl_update_hlhsr_id(ws);
   wsgl_update_projection(ws);
   wsgl_update_modelview(ws);
   wsgl_update_light_src_state(ws);

#ifdef DEBUG
   printf("Pop: id = %d, offset = %d\n",
          wsgl->cur_struct.id,
          wsgl->cur_struct.offset);
   printf("View:\n");
   phg_mat_print(wsgl->cur_struct.view_rep.map_matrix);
   printf("\n");
#endif

   if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
#ifdef DEBUG
     printf("\tPop name\n");
#endif
     glPopName();
      glPopName();
   }
}

/*******************************************************************************
 * wsgl_render_element
 *
 * DESCR:       Render element to current workstation rendering window
 * RETURNS:     N/A
 */
void wsgl_render_element(
                         Ws *ws,
                         El_handle el
                         )
{
  Pint_style style;
  Pmatrix3 mat3;
  Plocal_tran3 tran3;
  Wsgl_handle wsgl = ws->render_context;
  Pgse_elem gse_elem;
  Pgcolr highlight_color;
  Ws_output_ws *ows;
  Pfloat scalef = ws->hcsf;
  if (scalef == 0.0){
    scalef = 1.0;
    printf("WARNING: Scale factor is zero, setting it to 1.0");
  }
  update_cur_struct(ws);
  ows = &ws->out_ws;
  switch (el->eltype) {
  case PELEM_LABEL:
    break;
  case PELEM_PICK_ID:
#ifdef DEBUGINPUT
    printf("Render: found PICK ID %d\n", (int)PHG_INT(el));
#endif
    wsgl->cur_struct.pick_id = PHG_INT(el);
    store_cur_struct(ws);
    break;

  case PELEM_ADD_NAMES_SET:
    wsgl_add_names_set(ws, ELMT_CONTENT(el));
    break;

  case PELEM_REMOVE_NAMES_SET:
    wsgl_remove_names_set(ws, ELMT_CONTENT(el));
    break;

  case PELEM_HLHSR_ID:
    wsgl->cur_struct.hlhsr_id = PHG_INT(el);
    wsgl_update_hlhsr_id(ws);
    break;

  case PELEM_INDIV_ASF:
    wsgl_set_asf(&wsgl->cur_struct.ast, ELMT_CONTENT(el));
    break;

  case PELEM_INT_IND:
    wsgl_set_int_ind(ws, &wsgl->cur_struct.ast.bundl_group, PHG_INT(el));
    break;

  case PELEM_INT_COLR_IND:
    phg_get_colr_ind(ws,
                     &wsgl->cur_struct.ast.indiv_group.int_bundle.colr,
                     PHG_INT(el));
    break;

  case PELEM_INT_COLR:
    memcpy(&wsgl->cur_struct.ast.indiv_group.int_bundle.colr,
           ELMT_CONTENT(el),
           sizeof(Pgcolr));
    break;

  case PELEM_BACK_INT_COLR:
    memcpy(&wsgl->cur_struct.ast.indiv_group.int_bundle.back_colr,
           ELMT_CONTENT(el),
           sizeof(Pgcolr));
    break;

  case PELEM_INT_STYLE:
    wsgl->cur_struct.ast.indiv_group.int_bundle.style =
      (Pint_style) PHG_INT(el);
    break;

  case PELEM_BACK_INT_STYLE:
    wsgl->cur_struct.ast.indiv_group.int_bundle.back_style =
      (Pint_style) PHG_INT(el);
    break;

  case PELEM_INT_STYLE_IND:
    wsgl->cur_struct.ast.indiv_group.int_bundle.style_ind = PHG_INT(el);
    break;

  case PELEM_BACK_INT_STYLE_IND:
    wsgl->cur_struct.ast.indiv_group.int_bundle.back_style_ind =
      PHG_INT(el);
    break;

  case PELEM_EDGE_IND:
    wsgl_set_edge_ind(ws, &wsgl->cur_struct.ast.bundl_group, PHG_INT(el));
    break;

  case PELEM_EDGE_COLR_IND:
    phg_get_colr_ind(ws,
                     &wsgl->cur_struct.ast.indiv_group.edge_bundle.colr,
                     PHG_INT(el));
    break;

  case PELEM_EDGE_COLR:
    memcpy(&wsgl->cur_struct.ast.indiv_group.edge_bundle.colr,
           ELMT_CONTENT(el),
           sizeof(Pgcolr));
    break;

  case PELEM_EDGEWIDTH:
    wsgl->cur_struct.ast.indiv_group.edge_bundle.width = PHG_FLOAT(el)*scalef;
    break;

  case PELEM_EDGETYPE:
    wsgl->cur_struct.ast.indiv_group.edge_bundle.type = PHG_INT(el);
    break;

  case PELEM_EDGE_FLAG:
    wsgl->cur_struct.ast.indiv_group.edge_bundle.flag =
      (Pedge_flag) PHG_INT(el);
    break;

  case PELEM_MARKER_IND:
    wsgl_set_marker_ind(ws, &wsgl->cur_struct.ast.bundl_group, PHG_INT(el));
    break;

  case PELEM_MARKER_COLR_IND:
    phg_get_colr_ind(ws,
                     &wsgl->cur_struct.ast.indiv_group.marker_bundle.colr,
                     PHG_INT(el));
    break;

  case PELEM_MARKER_COLR:
    memcpy(&wsgl->cur_struct.ast.indiv_group.marker_bundle.colr,
           ELMT_CONTENT(el),
           sizeof(Pgcolr));
    break;

  case PELEM_MARKER_SIZE:
    wsgl->cur_struct.ast.indiv_group.marker_bundle.size = PHG_FLOAT(el)*scalef;
    break;

  case PELEM_MARKER_TYPE:
    wsgl->cur_struct.ast.indiv_group.marker_bundle.type = PHG_INT(el);
    break;

  case PELEM_TEXT_IND:
    wsgl_set_text_ind(ws, &wsgl->cur_struct.ast.bundl_group, PHG_INT(el));
    break;

  case PELEM_TEXT_COLR_IND:
    phg_get_colr_ind(ws,
                     &wsgl->cur_struct.ast.indiv_group.text_bundle.colr,
                     PHG_INT(el));
    break;

  case PELEM_TEXT_COLR:
    memcpy(&wsgl->cur_struct.ast.indiv_group.text_bundle.colr,
           ELMT_CONTENT(el),
           sizeof(Pgcolr));
    break;

  case PELEM_TEXT_FONT:
    wsgl->cur_struct.ast.indiv_group.text_bundle.font = PHG_INT(el);
    break;

  case PELEM_TEXT_PREC:
    wsgl->cur_struct.ast.indiv_group.text_bundle.prec =
      (Ptext_prec) PHG_INT(el);
    break;

  case PELEM_CHAR_HT:
    wsgl->cur_struct.ast.char_ht = PHG_FLOAT(el);
    break;

  case PELEM_CHAR_EXPAN:
    wsgl->cur_struct.ast.indiv_group.text_bundle.char_expan =
      PHG_FLOAT(el);
    break;

  case PELEM_CHAR_SPACE:
    wsgl->cur_struct.ast.indiv_group.text_bundle.char_space =
      PHG_FLOAT(el);
    break;

  case PELEM_TEXT_PATH:
    wsgl->cur_struct.ast.text_path =
      (Ptext_path) PHG_INT(el);
    break;

  case PELEM_TEXT_ALIGN:
    memcpy(&wsgl->cur_struct.ast.text_align,
           ELMT_CONTENT(el),
           sizeof(Ptext_align));
    break;

  case PELEM_CHAR_UP_VEC:
    memcpy(&wsgl->cur_struct.ast.char_up_vec,
           ELMT_CONTENT(el),
           sizeof(Pvec));
    break;

  case PELEM_LINE_IND:
    wsgl_set_line_ind(ws, &wsgl->cur_struct.ast.bundl_group, PHG_INT(el));
    break;

  case PELEM_LINE_COLR_IND:
    if (check_highlight_primitive(ws)){
      phg_get_colr_ind(ws,
                       &wsgl->cur_struct.ast.highlighting_color,
                       PHG_INT(el));
    }
    else
      {
        phg_get_colr_ind(ws,
                         &wsgl->cur_struct.ast.indiv_group.line_bundle.colr,
                         PHG_INT(el));
      }
    break;

  case PELEM_LINE_COLR:
    memcpy(&wsgl->cur_struct.ast.indiv_group.line_bundle.colr,
           ELMT_CONTENT(el),
           sizeof(Pgcolr));
    break;

  case PELEM_LINEWIDTH:
    wsgl->cur_struct.ast.indiv_group.line_bundle.width = PHG_FLOAT(el)*scalef;
    break;

  case PELEM_LINETYPE:
    wsgl->cur_struct.ast.indiv_group.line_bundle.type = PHG_INT(el);
    break;

  case PELEM_FILL_AREA:
    if (check_draw_primitive(ws)) {
      if (wsgl_get_int_style(&wsgl->cur_struct.ast) != PSTYLE_EMPTY) {
        wsgl_fill_area(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
      }
      if (wsgl_get_edge_flag(&wsgl->cur_struct.ast) == PEDGE_ON) {
        wsgl_edge_area(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
      }
    }
    break;

  case PELEM_FILL_AREA_SET:
    if (check_draw_primitive(ws)) {
      if (wsgl_get_int_style(&wsgl->cur_struct.ast) != PSTYLE_EMPTY) {
        wsgl_fill_area_set(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
      }
      if (wsgl_get_edge_flag(&wsgl->cur_struct.ast) == PEDGE_ON) {
        wsgl_edge_area_set(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
      }
    }
    break;

  case PELEM_POLYLINE:
    if (check_draw_primitive(ws)) {
      wsgl_polyline(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
    }
    break;

  case PELEM_POLYMARKER:
    if (check_draw_primitive(ws)) {
      wsgl_polymarker(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
    }
    break;

  case PELEM_FILL_AREA3:
    if (check_draw_primitive(ws)) {
      style = wsgl_get_int_style(&wsgl->cur_struct.ast);
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
        if (style == PSTYLE_EMPTY || style == PSTYLE_HOLLOW) {
          wsgl_clear_area3(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
        }
      }
      if (style != PSTYLE_EMPTY) {
        if (wsgl->cur_struct.ast.cull_mode != PCULL_BACKFACE) {
          if (wsgl->cur_struct.ast.disting_mode == PDISTING_YES) {
            glEnable(GL_CULL_FACE);
            wsgl_back_area3(ws,
                            ELMT_CONTENT(el),
                            &wsgl->cur_struct.ast);
            glDisable(GL_CULL_FACE);
          }
        }
        if (wsgl->cur_struct.ast.cull_mode != PCULL_FRONTFACE) {
          if (wsgl->cur_struct.ast.disting_mode == PDISTING_YES) {
            glEnable(GL_CULL_FACE);
            wsgl_fill_area3(ws,
                            ELMT_CONTENT(el),
                            &wsgl->cur_struct.ast);
            glDisable(GL_CULL_FACE);
          }
          else {
            wsgl_fill_area3(ws,
                            ELMT_CONTENT(el),
                            &wsgl->cur_struct.ast);
          }
        }
      }
      if (wsgl_get_edge_flag(&wsgl->cur_struct.ast) == PEDGE_ON) {
        wsgl_edge_area3(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
      }
    }
    break;

  case PELEM_FILL_AREA_SET3:
    if (check_draw_primitive(ws)) {
      style = wsgl_get_int_style(&wsgl->cur_struct.ast);
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
        if (style == PSTYLE_EMPTY || style == PSTYLE_HOLLOW) {
          wsgl_clear_area_set3(ws,
                               ELMT_CONTENT(el),
                               &wsgl->cur_struct.ast);
        }
      }
      if (style != PSTYLE_EMPTY) {
        wsgl_fill_area_set3(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
      }
      if (wsgl_get_edge_flag(&wsgl->cur_struct.ast) == PEDGE_ON) {
        wsgl_edge_area_set3(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
      }
    }
    break;

  case PELEM_FILL_AREA_SET_DATA:
    if (check_draw_primitive(ws)) {
      style = wsgl_get_int_style(&wsgl->cur_struct.ast);
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
        if (style == PSTYLE_EMPTY || style == PSTYLE_HOLLOW) {
          wsgl_clear_area_set_data(ws,
                                   ELMT_CONTENT(el),
                                   &wsgl->cur_struct.ast);
        }
      }
      if (style != PSTYLE_EMPTY) {
        if (wsgl->cur_struct.ast.cull_mode != PCULL_BACKFACE) {
          if (wsgl->cur_struct.ast.disting_mode == PDISTING_YES) {
            glEnable(GL_CULL_FACE);
            wsgl_fill_area_set_data_back(ws,
                                         ELMT_CONTENT(el),
                                         &wsgl->cur_struct.ast);
            glDisable(GL_CULL_FACE);
          }
        }
        if (wsgl->cur_struct.ast.cull_mode != PCULL_FRONTFACE) {
          if (wsgl->cur_struct.ast.disting_mode == PDISTING_YES) {
            glEnable(GL_CULL_FACE);
            wsgl_fill_area_set_data_front(ws,
                                          ELMT_CONTENT(el),
                                          &wsgl->cur_struct.ast);
            glDisable(GL_CULL_FACE);
          }
          else {
            wsgl_fill_area_set_data_front(ws,
                                          ELMT_CONTENT(el),
                                          &wsgl->cur_struct.ast);
          }
        }
      }
      if (wsgl_get_edge_flag(&wsgl->cur_struct.ast) == PEDGE_ON) {
        wsgl_edge_area_set_data(ws,
                                ELMT_CONTENT(el),
                                &wsgl->cur_struct.ast);
      }
    }
    break;

  case PELEM_FILL_AREA_SET3_DATA:
    if (check_draw_primitive(ws)) {
      style = wsgl_get_int_style(&wsgl->cur_struct.ast);
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
        if (style == PSTYLE_EMPTY || style == PSTYLE_HOLLOW) {
          wsgl_clear_area_set3_data(ws,
                                    ELMT_CONTENT(el),
                                    &wsgl->cur_struct.ast);
        }
      }
      if (style != PSTYLE_EMPTY) {
        if (wsgl->cur_struct.ast.cull_mode != PCULL_BACKFACE) {
          if (wsgl->cur_struct.ast.disting_mode == PDISTING_YES) {
            glEnable(GL_CULL_FACE);
            wsgl_fill_area_set3_data_back(ws,
                                          ELMT_CONTENT(el),
                                          &wsgl->cur_struct.ast);
            glDisable(GL_CULL_FACE);
          }
        }
        if (wsgl->cur_struct.ast.cull_mode != PCULL_FRONTFACE) {
          if (wsgl->cur_struct.ast.disting_mode == PDISTING_YES) {
            glEnable(GL_CULL_FACE);
            wsgl_fill_area_set3_data_front(ws,
                                           ELMT_CONTENT(el),
                                           &wsgl->cur_struct.ast);
            glDisable(GL_CULL_FACE);
          }
          else {
            wsgl_fill_area_set3_data_front(ws,
                                           ELMT_CONTENT(el),
                                           &wsgl->cur_struct.ast);
          }
        }
      }
      if (wsgl_get_edge_flag(&wsgl->cur_struct.ast) == PEDGE_ON) {
        wsgl_edge_area_set3_data(ws,
                                 ELMT_CONTENT(el),
                                 &wsgl->cur_struct.ast);
      }
    }
    break;

  case PELEM_SET_OF_FILL_AREA_SET3_DATA:
    if (check_draw_primitive(ws)) {
      style = wsgl_get_int_style(&wsgl->cur_struct.ast);
      if (wsgl->cur_struct.hlhsr_id == PHIGS_HLHSR_ID_ON) {
        if (style == PSTYLE_EMPTY || style == PSTYLE_HOLLOW) {
          wsgl_set_of_clear_area_set3_data(ws,
                                           ELMT_CONTENT(el),
                                           &wsgl->cur_struct.ast);
        }
      }
      if (style != PSTYLE_EMPTY) {
        if (wsgl->cur_struct.ast.cull_mode != PCULL_BACKFACE) {
          if (wsgl->cur_struct.ast.disting_mode == PDISTING_YES) {
            glEnable(GL_CULL_FACE);
            wsgl_set_of_fill_area_set3_data_back(ws,
                                                 ELMT_CONTENT(el),
                                                 &wsgl->cur_struct.ast);
            glDisable(GL_CULL_FACE);
          }
        }
        if (wsgl->cur_struct.ast.cull_mode != PCULL_FRONTFACE) {
          if (wsgl->cur_struct.ast.disting_mode == PDISTING_YES) {
            glEnable(GL_CULL_FACE);
            wsgl_set_of_fill_area_set3_data_front(ws,
                                                  ELMT_CONTENT(el),
                                                  &wsgl->cur_struct.ast);
            glDisable(GL_CULL_FACE);
          }
          else {
            wsgl_set_of_fill_area_set3_data_front(ws,
                                                  ELMT_CONTENT(el),
                                                  &wsgl->cur_struct.ast);
          }
        }
      }
      if (wsgl_get_edge_flag(&wsgl->cur_struct.ast) == PEDGE_ON) {
        wsgl_set_of_edge_area_set3_data(ws,
                                        ELMT_CONTENT(el),
                                        &wsgl->cur_struct.ast);
      }
    }
    break;

  case PELEM_POLYLINE3:
    if (check_draw_primitive(ws)) {
      wsgl_polyline3(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
    }
    break;

  case PELEM_POLYMARKER3:
    if (check_draw_primitive(ws)) {
      wsgl_polymarker3(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
    }
    break;

  case PELEM_ANNO_TEXT_REL:
    {
      wsgl_anno_text_rel(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast, wsgl->cur_struct.view_rep.ori_matrix);
    }
    break;

  case PELEM_ANNO_TEXT_REL3:
    {
      wsgl_anno_text_rel3(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast, wsgl->cur_struct.view_rep.ori_matrix);
    }
    break;

  case PELEM_ANNO_CHAR_HT:
    wsgl->cur_struct.ast.anno_char_ht = PHG_FLOAT(el);
    break;

  case PELEM_ANNO_ALIGN:
    memcpy(&wsgl->cur_struct.ast.anno_text_align,
           ELMT_CONTENT(el),
           sizeof(Ptext_align));
    break;

  case PELEM_ANNO_PATH:
    wsgl->cur_struct.ast.anno_text_path =
      (Ptext_path) PHG_INT(el);
    break;

  case PELEM_ANNO_CHAR_UP_VEC:
    memcpy(&wsgl->cur_struct.ast.anno_char_up_vec,
           ELMT_CONTENT(el),
           sizeof(Pvec));
    break;

  case PELEM_TEXT:
    wsgl_text(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
    break;

  case PELEM_TEXT3:
    wsgl_text3(ws, ELMT_CONTENT(el), &wsgl->cur_struct.ast);
    break;

  case PELEM_GLOBAL_MODEL_TRAN3:
    phg_mat_pack(mat3, (Pfloat *) ELMT_CONTENT(el));
    phg_mat_copy(wsgl->cur_struct.global_tran, mat3);
    wsgl_update_modelview(ws);
    break;

  case PELEM_LOCAL_MODEL_TRAN3:
    phg_get_local_tran3(&tran3, ELMT_CONTENT(el));
    switch (tran3.compose_type) {
    case PTYPE_PRECONCAT:
      phg_mat_mul(wsgl->cur_struct.local_tran,
                  wsgl->cur_struct.local_tran,
                  tran3.matrix);
      break;
    case PTYPE_POSTCONCAT:
      phg_mat_mul(wsgl->cur_struct.local_tran,
                  tran3.matrix,
                  wsgl->cur_struct.local_tran);
      break;
    case PTYPE_REPLACE:
    default:
      phg_mat_copy(wsgl->cur_struct.local_tran,
                   tran3.matrix);
      break;
    }
    wsgl_update_modelview(ws);
    break;

  case PELEM_VIEW_IND:
    wsgl_set_view_ind(ws, PHG_INT(el));
    break;

  case PELEM_LIGHT_SRC_STATE:
    wsgl_set_light_src_state(ws, ELMT_CONTENT(el));
    break;

  case PELEM_INT_SHAD_METH:
    wsgl->cur_struct.ast.indiv_group.int_bundle.shad_meth = PHG_INT(el);
    break;

  case PELEM_BACK_INT_SHAD_METH:
    wsgl->cur_struct.ast.indiv_group.int_bundle.back_shad_meth =
      PHG_INT(el);
    break;

  case PELEM_INT_REFL_EQN:
    wsgl->cur_struct.ast.indiv_group.int_bundle.refl_eqn = PHG_INT(el);
    break;

  case PELEM_BACK_INT_REFL_EQN:
    wsgl->cur_struct.ast.indiv_group.int_bundle.back_refl_eqn =
      PHG_INT(el);
    break;

  case PELEM_REFL_PROPS:
    memcpy(&wsgl->cur_struct.ast.indiv_group.int_bundle.refl_props,
           ELMT_CONTENT(el),
           sizeof(Prefl_props));
    break;

  case PELEM_BACK_REFL_PROPS:
    memcpy(&wsgl->cur_struct.ast.indiv_group.int_bundle.back_refl_props,
           ELMT_CONTENT(el),
           sizeof(Prefl_props));
    break;

  case PELEM_FACE_DISTING_MODE:
    wsgl->cur_struct.ast.disting_mode = (Pdisting_mode) PHG_INT(el);
    break;

  case PELEM_FACE_CULL_MODE:
    wsgl->cur_struct.ast.cull_mode = (Pcull_mode) PHG_INT(el);
    break;

  case PELEM_GSE:
    memcpy(&gse_elem, ELMT_CONTENT(el), sizeof(Pgse_elem));
    if (gse_elem.gse_type == PGSE_ID_HIGHLIGHT_COLOR) {
      wsgl->cur_struct.ast.highlighting_color = gse_elem.gse_data.colr.highlight_colr;
    }
    break;

  case PELEM_MODEL_CLIP_IND:
    wsgl_set_clip_ind(ws, PHG_INT(el));
    break;

  case PELEM_MODEL_CLIP_VOL3:
    wsgl_set_clip_vol3(ws, (char*)ELMT_CONTENT(el));
    break;

  case PELEM_ALPHA_CHANNEL:
    wsgl_set_alpha_channel(ws, PHG_FLOAT(el));
    break;

  default:
    css_print_eltype(el->eltype);
    printf(" not processed\n");
    break;
  }
}

/*******************************************************************************
 * wsgl_set_filter
 *
 * DESCR:       Set filter
 * RETURNS:     N/A
 */
void wsgl_set_filter(
                     Ws *ws,
                     Phg_args_flt_type type,
                     Nameset incl,
                     Nameset excl
                     )
{
  Wsgl_handle wsgl = ws->render_context;

  switch (type) {
  case PHG_ARGS_FLT_INVIS:
    wsgl->invis_filter.used = TRUE;
    wsgl->invis_filter.incl = incl;
    wsgl->invis_filter.excl = excl;
#ifdef DEBUG
    printf("Include filter:\n");
    phg_nset_print(wsgl->invis_filter.incl);
    printf("Exclude filter:\n");
    phg_nset_print(wsgl->invis_filter.excl);
#endif
    break;

  case PHG_ARGS_FLT_PICK:
    wsgl->pick_filter.used = TRUE;
    wsgl->pick_filter.incl = incl;
    wsgl->pick_filter.excl = excl;
#ifdef DEBUG
    printf("Include filter:\n");
    phg_nset_print(wsgl->pick_filter.incl);
    printf("Exclude filter:\n");
    phg_nset_print(wsgl->pick_filter.excl);
#endif
    break;

  case PHG_ARGS_FLT_HIGH:
    wsgl->highl_filter.used = TRUE;
    wsgl->highl_filter.incl = incl;
    wsgl->highl_filter.excl = excl;
#ifdef DEBUG
    printf("Include filter:\n");
    phg_nset_print(wsgl->highl_filter.incl);
    printf("Exclude filter:\n");
    phg_nset_print(wsgl->highl_filter.excl);
#endif
    break;

  default:
    /* TODO: Other filter types */
    break;
  }
}

/*******************************************************************************
 * wsgl_begin_pick
 *
 * DESCR:       Begin pick process
 * RETURNS:     N/A
 */
void wsgl_begin_pick(
                     Ws *ws,
                     Ws_hit_box *box
                     )
{
  GLint vp[4];
  Pvec3 v;
  Pmatrix3 trans, scale;
  Wsgl_handle wsgl = ws->render_context;
  wsgl->render_mode = WS_RENDER_MODE_SELECT;

#ifdef DEBUGINP
  printf("WSGL Begin pick\n");
#endif
  if (ws->drawable_id != 0){
#ifdef DEBUGINP
    printf("WSGL Begin Pick: selecting current contex\n");
#endif
    glXMakeContextCurrent(ws->display, ws->drawable_id, ws->drawable_id, ws->glx_context);
  }
#ifdef DEBUGINP
  else  printf("WSGL Begin Pick: drawable ID is zero ?\n");
#endif

  glGetIntegerv(GL_VIEWPORT, vp);
  v.delta_x = ((float) vp[2] - 2.0 * ((float) box->x - (float) vp[0])) /
    box->distance;
  v.delta_y = ((float) vp[3] - 2.0 * ((float) box->y - (float) vp[1])) /
    box->distance;
  v.delta_z = 0.0;
  phg_mat_translate(trans, &v);

  v.delta_x = (float) vp[2] / box->distance;
  v.delta_y = (float) vp[3] / box->distance;
  v.delta_z = 1.0;
  phg_mat_scale(scale, &v);

  phg_mat_mul(wsgl->pick_tran, trans, scale);
#ifdef DEBUGINP
  phg_mat_print(wsgl->pick_tran);
  printf("\n");
#endif

  init_rendering_state(ws);
  glSelectBuffer(wsgl->select_size, wsgl->select_buf);
#ifdef DEBUGINP
  printf("WSGL begin pick: set render mode to select\n");
  printf("WSGL begin pick: buffer size is %d at %p\n", wsgl->select_size, (void*)wsgl->select_buf);
#endif
  glRenderMode(GL_SELECT);
  glInitNames();
}

/*******************************************************************************
 * wsgl_end_pick
 *
 * DESCR:       End pick process
 * RETURNS:     N/A
 */
void wsgl_end_pick(
                   Ws *ws,
                   Pint *err_ind,
                   Pint *depth,
                   Ws_pick_elmt **elmts
                   )
{
  Pint hits;
  Pint i;
  GLuint z, names;
#ifdef DEBUGINP
  Pint j;
  printf("WSGL end pick\n");
#endif

  Wsgl_handle wsgl = ws->render_context;
  GLuint *ptr = wsgl->select_buf;
  GLuint *match = NULL;
  Ws_pick_elmt *data = NULL;
  GLuint zmin = UINT_MAX;
  wsgl->render_mode = WS_RENDER_MODE_DRAW;
  glFlush();
#ifdef DEBUGINP
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  printf("Viewport: %d %d %d %d\n", viewport[0], viewport[1], viewport[2], viewport[3]);
#endif
  hits = glRenderMode(GL_RENDER);
#ifdef DEBUGINP
  printf("Number of hits: %d\n", hits);
#endif
  for (i = 0; i < hits; i++) {

    names = *ptr / 2;
    ptr++;
    z = *ptr;
    if (z < zmin) {
      zmin = z;
      match = ptr - 1;
    }
#ifdef DEBUGINP
    ptr += 2;
    printf("Number of name(s) for hit #%d is %d\tAt: %x\n", i, names, z);
    for (j = 0; j < names; j++) {
      printf("\tStruct: %d", *ptr);
      ptr++;
      printf("\tOffset: %d\tPick ID: %d\n",
             (*ptr & 0xffff0000) >> 16, *ptr & 0xffff);
      ptr++;
    }
#else
    ptr += 2 * names + 2;
#endif
  }

  if (match != NULL) {
    names = *match / 2;
#ifdef DEBUGINP
    printf("\nThe matching hit with %d name(s)\n", names);
#endif
    data = (Ws_pick_elmt *) malloc(sizeof(Ws_pick_elmt) * names);
    if (data == NULL) {
      *depth   = 0;
      *elmts   = NULL;
      *err_ind = ERR900;
    }
    else {
      match += 3;
      for (i = 0; i < names; i++) {
        data[i].sid = *match;
        match++;
        data[i].pickid = *match & 0xffff;
        data[i].offset = (*match & 0xffff0000) >> 16;
#ifdef DEBUGINP
        printf("\tStruct: %d\tOffset: %d\tPick ID: %d\n",
               data[i].sid, data[i].offset, data[i].pickid);
#endif
        match++;
      }
      *depth   = names;
      *elmts   = data;
      *err_ind = 0;
    }
  }
  else {
    *depth   = 0;
    *err_ind = 0;
  }
}
