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
#include <math.h>
#ifdef GLEW
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif
#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "private/wsglP.h"

extern GLint shading_mode;
extern GLint ModelViewMatrix, ProjectionMatrix;
extern GLint alpha_channel;

/*******************************************************************************
 * wsgl_set_matrix
 *
 * DESCR:    Setup matrix
 * RETURNS:    N/A
 */

static void wsgl_set_matrix(
                            Pmatrix3 mat,
                            int mult
                            )
{
  int i, j;
  GLfloat m[16];
  GLfloat *mp = &m[0];

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      *mp = mat[j][i];
      mp++;
    }
  }

  if (mult) {
    glMultMatrixf(m);
  }
  else {
    glLoadMatrixf(m);
  }
}

static void wsgl_set_model_view_matrix(
                                       Pmatrix3 mat
                                       )
{
  int i, j;
  GLfloat m[16];
  GLfloat *mp = &m[0];

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      *mp = mat[j][i];
      mp++;
    }
  }
  glUniformMatrix4fv(ModelViewMatrix, 1, FALSE, m);
}

static void wsgl_set_projection_matrix(
                                       Pmatrix3 mat
                                       )
{
  int i, j;
  GLfloat m[16];
  GLfloat *mp = &m[0];
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      *mp = mat[j][i];
      mp++;
    }
  }
  glUniformMatrix4fv(ProjectionMatrix, 1, FALSE, m);
}

/*******************************************************************************
 * wsgl_update_projection
 *
 * DESCR:    Update projection matrix
 * RETURNS:    N/A
 */
void wsgl_update_projection(
                            Ws *ws
                            )
{
  Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
  printf("Update projection\n");
#endif

  glMatrixMode(GL_PROJECTION);
  if (wsgl->render_mode == WS_RENDER_MODE_SELECT) {
    phg_mat_mul(wsgl->model_tran,
                wsgl->pick_tran,
                wsgl->cur_struct.view_rep.map_matrix);
    if (wsgl_use_shaders){
      wsgl_set_projection_matrix(wsgl->model_tran);
    } else {
      wsgl_set_matrix(wsgl->model_tran, FALSE);
    }
  } else {
    if (wsgl_use_shaders){
      wsgl_set_projection_matrix(wsgl->cur_struct.view_rep.map_matrix);
    } else {
      wsgl_set_matrix(wsgl->cur_struct.view_rep.map_matrix, FALSE);
    }
  }
}

/*******************************************************************************
 * wsgl_update_modelview
 *
 * DESCR:    Update modelview matrix
 * RETURNS:    N/A
 */

void wsgl_update_modelview(
                           Ws *ws
                           )
{
  Wsgl_handle wsgl = ws->render_context;

#ifdef DEBUG
  printf("Update modelview\n");
  printf("Orientation matrix is :\n");
  printf("%e %e %e\n", (double) *wsgl->cur_struct.view_rep.ori_matrix[0,0], (double) *wsgl->cur_struct.view_rep.ori_matrix[0,1], (double) *wsgl->cur_struct.view_rep.ori_matrix[0,2]);
  printf("%e %e %e\n", (double) *wsgl->cur_struct.view_rep.ori_matrix[1,0], (double) *wsgl->cur_struct.view_rep.ori_matrix[1,1], (double) *wsgl->cur_struct.view_rep.ori_matrix[1,2]);
  printf("%e %e %e\n", (double) *wsgl->cur_struct.view_rep.ori_matrix[2,0], (double) *wsgl->cur_struct.view_rep.ori_matrix[2,1], (double) *wsgl->cur_struct.view_rep.ori_matrix[2,2]);
#endif


  glMatrixMode(GL_MODELVIEW);
  phg_mat_mul(wsgl->composite_tran,
              wsgl->cur_struct.global_tran,
              wsgl->cur_struct.local_tran);
  phg_mat_mul(wsgl->model_tran,
              wsgl->cur_struct.view_rep.ori_matrix,
              wsgl->composite_tran);
  if (wsgl_use_shaders){
    wsgl_set_model_view_matrix(wsgl->model_tran);
  } else {
    wsgl_set_matrix(wsgl->model_tran, FALSE);
  }
}

/*******************************************************************************
 * wsgl_set_view_ind
 *
 * DESCR:    Setup view
 * RETURNS:    N/A
 */
void wsgl_set_view_ind(
                       Ws *ws,
                       Pint ind
                       )
{
  Phg_ret ret;
  Wsgl_handle wsgl = ws->render_context;

  (*ws->inq_representation)(ws,
                            ind,
                            PINQ_REALIZED,
                            PHG_ARGS_VIEWREP,
                            &ret);
  if (ret.err == 0) {
    memcpy(&wsgl->cur_struct.view_rep,
           &ret.data.rep.viewrep,
           sizeof(Pview_rep3));
    wsgl_update_projection(ws);
    wsgl_update_modelview(ws);
  }
}

/*******************************************************************************
 * wsgl_set_clip_ind
 *
 * DESCR:    Setup clip index
 * RETURNS:    N/A
 */

void wsgl_set_clip_ind(
                       Ws *ws,
                       Pint ind
                       )
{
  Phg_ret ret;
  Wsgl_handle wsgl = ws->render_context;
  if (ind == 1) {
    glEnable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
  } else if (ind == 2) {
    glEnable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
  } else {
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
  }
}

/*******************************************************************************
 * wsgl_set_clip_vol3
 *
 * DESCR:    Setup view
 * RETURNS:    N/A
 */
void wsgl_set_clip_vol3(
                        Ws *ws,
                        char * el_data,
                        Pmatrix3 ModelviewMatrix
                        )
{
  Phg_ret ret;
  int op, num;
  int * int_data = (int*) el_data;
  Phalf_space3 * list;
  Phalf_space3 volume0, volume1;
  Ppoint4 norm3;
  Ppoint4 nn0, nn1;
  Ppoint3 vol3;
  Ppoint3 pt0, pt1; /* second plane if any */
  Pmatrix3 wc2evnorm, wc2ev2;

  /* for norms, invert and transpose */
  phg_mat_copy(wc2evnorm, ModelviewMatrix);
  phg_mat_copy(wc2ev2, ModelviewMatrix);

  phg_mat_inv(wc2evnorm);
  phg_mat_transpose(wc2evnorm);

  op = int_data[0];
  num = int_data[1];
  list = (Phalf_space3 *)(&int_data[2]);
  if (num >=1 && num <=2){
    /* first plane */
    volume0 = list[0];

    vol3.x = volume0.point.x;
    vol3.y = volume0.point.y;
    vol3.z = volume0.point.z;
    phg_tranpt3(&vol3, wc2ev2, &pt0);

    norm3.x = volume0.norm.delta_x;
    norm3.y = volume0.norm.delta_y;
    norm3.z = volume0.norm.delta_z;
    norm3.w = 0.0;
    phg_tranvec4(&norm3, wc2evnorm, &nn0);

    /* normalise */
    float len=sqrt(nn0.x*nn0.x+nn0.y*nn0.y+nn0.z*nn0.z);
    if (len>0){
      nn0.x = nn0.x/len;
      nn0.y = nn0.y/len;
      nn0.z = nn0.z/len;
    }
#ifdef DEBUGCL
    printf("Plane0: norm in %f %f %f %flength %f\n",
           norm3.x, norm3.y, norm3.z, norm3.w, sqrt(norm3.x*norm3.x+norm3.y*norm3.y+norm3.z*norm3.z));
    printf("Plane0: norm out %f %f %f %f length %f\n",
           nn0.x, nn0.y, nn0.z, nn0.w, sqrt(nn0.x*nn0.x+nn0.y*nn0.y+nn0.z*nn0.z));
    printf("Plane0: point in %f %f %f length %f\n",
           vol3.x, vol3.y, vol3.z, sqrt(vol3.x*vol3.x+vol3.y*vol3.y+vol3.z*vol3.z));
    printf("Plane0: point out %f %f %f length %f\n",
           pt0.x, pt0.y, pt0.z, sqrt(pt0.x*pt0.x+pt0.y*pt0.y+pt0.z*pt0.z));
#endif

    if (2 ==num){
      /* first plane */
      volume1 = list[1];
      /* take a local copy of the data */
      vol3.x = volume1.point.x;
      vol3.y = volume1.point.y;
      vol3.z = volume1.point.z;
      phg_tranpt3(&vol3, wc2ev2, &pt1);

      norm3.x = volume1.norm.delta_x;
      norm3.y = volume1.norm.delta_y;
      norm3.z = volume1.norm.delta_z;
      norm3.w = 0.0;
      phg_tranvec4(&norm3, wc2evnorm, &nn1);

      float len=sqrt(nn1.x*nn1.x+nn1.y*nn1.y+nn1.z*nn1.z);
      if (len>0){
        nn1.x = nn1.x/len;
        nn1.y = nn1.y/len;
        nn1.z = nn1.z/len;
      }
#ifdef DEBUGCL
      printf("Plane1: norm in %f %f %f %flength %f\n",
             norm3.x, norm3.y, norm3.z, norm3.w, sqrt(norm3.x*norm3.x+norm3.y*norm3.y+norm3.z*norm3.z));
      printf("Plane1: norm out %f %f %f %f length %f\n",
             nn1.x, nn1.y, nn1.z, nn1.w, sqrt(nn1.x*nn1.x+nn1.y*nn1.y+nn1.z*nn1.z));
      printf("Plane1: point in %f %f %f length %f\n",
             vol3.x, vol3.y, vol3.z, sqrt(vol3.x*vol3.x+vol3.y*vol3.y+vol3.z*vol3.z));
      printf("Plane1: point out %f %f %f length %f\n",
             pt1.x, pt1.y, pt1.z, sqrt(pt1.x*pt1.x+pt1.y*pt1.y+pt1.z*pt1.z));
#endif

    }
  }
  if (num >=1 ){
    GLdouble eqn0[4] = {
      nn0.x, nn0.y, nn0.z,
      -(nn0.x * pt0.x + nn0.y * pt0.y + nn0.z * pt0.z)
    };
    glClipPlane(GL_CLIP_PLANE0, eqn0);
    if (num == 2){
      GLdouble eqn1[4] = {
        nn1.x, nn1.y, nn1.z,
        -(nn1.x * pt1.x + nn1.y * pt1.y + nn1.z * pt1.z)
      };
      glClipPlane(GL_CLIP_PLANE1, eqn1);
    }
  }
}

/*******************************************************************************
 * wsgl_update_hlhsr_id
 *
 * DESCR:    Update depth buffer checking flag
 * RETURNS:    N/A
 */
void wsgl_update_hlhsr_id(
                          Ws *ws
                          )
{
  Wsgl_handle wsgl = ws->render_context;

  switch(wsgl->cur_struct.hlhsr_id) {
  case PHIGS_HLHSR_ID_OFF:
    glDepthFunc(GL_ALWAYS);
    break;

  case PHIGS_HLHSR_ID_ON:
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    break;

  case PHIGS_HLHSR_ID_ON_NZ:
    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_TEST);
    break;

  default:
    break;
  }
}

/*******************************************************************************
 * wsgl_set_asf
 *
 * DESCR:    Setup asf
 * RETURNS:    N/A
 */
void wsgl_set_asf(
                  Ws_attr_st *ast,
                  void *asf_info
                  )
{
  Pasf_info *data = (Pasf_info *) asf_info;

  if (data->source == PASF_INDIV) {
    phg_nset_name_set(&ast->asf_nameset, data->id);
  }
  else {
    phg_nset_name_clear(&ast->asf_nameset, data->id);
  }
}

/*******************************************************************************
 * wsgl_set_colr
 *
 * DESCR:    Set colour value
 * RETURNS:    N/A
 */
void wsgl_set_colr(
                   Pint colr_type,
                   Pcoval *colr
                   )
{
  int* crash = 0;
#ifdef DEBUGA
  printf("wsgl_set_colr: setting color of type %d\n",  colr_type);
#endif
  switch(colr_type) {
  case PINDIRECT:
    glIndexi(colr->ind);
    break;

  case PMODEL_RGB:
    if (wsgl_use_shaders) {
      glVertexAttrib4f(vCOLOR,
                       colr->direct.rgb.red,
                       colr->direct.rgb.green,
                       colr->direct.rgb.blue,
                       1.0);
    } else {
      glColor4f(colr->direct.rgb.red,
                colr->direct.rgb.green,
                colr->direct.rgb.blue,
                1.0);
    }
    break;

  case PMODEL_RGBA:
    if (wsgl_use_shaders){
      glVertexAttrib4f(vCOLOR,
                       colr->direct.rgba.red,
                       colr->direct.rgba.green,
                       colr->direct.rgba.blue,
                       colr->direct.rgba.alpha);
    } else {
#ifdef DEBUGA
      printf("DEBUG wsgl_set_colr: Color model rgba, %f %f %f %f\n",
             colr->direct.rgba.red,
             colr->direct.rgba.green,
             colr->direct.rgba.blue,
             colr->direct.rgba.alpha);
      if (colr->direct.rgba.alpha <= 0 || colr->direct.rgba.alpha > 1.) {
        printf("Alpha value out of bounds. Crashing.\n");
        *crash = 1;
      }
#endif
      glColor4f(colr->direct.rgba.red,
                colr->direct.rgba.green,
                colr->direct.rgba.blue,
                colr->direct.rgba.alpha);
    }
    break;

  default:
    break;
  }
}

/*******************************************************************************
 * wsgl_set_gcolr
 *
 * DESCR:    Set colour
 * RETURNS:    N/A
 */
void wsgl_set_gcolr(
                    Pgcolr *gcolr
                    )
{
  switch(gcolr->type) {
  case PINDIRECT:
    glIndexi(gcolr->val.ind);
    break;

  case PMODEL_RGB:
    if (wsgl_use_shaders) {
      glVertexAttrib4f(vCOLOR,
                       gcolr->val.general.x,
                       gcolr->val.general.y,
                       gcolr->val.general.z,
                       1.0);
    } else {
      glColor4f(gcolr->val.general.x,
                gcolr->val.general.y,
                gcolr->val.general.z,
                1.0);
    }
    break;

  case PMODEL_RGBA:
    if (wsgl_use_shaders) {
#ifdef DEBUGA
      printf("wsgl_set_gcolr setting %f %f %f %f\n",
             gcolr->val.general.x,
             gcolr->val.general.y,
             gcolr->val.general.z,
             gcolr->val.general.a
             );
#endif
      glVertexAttrib4f(vCOLOR,
                       gcolr->val.general.x,
                       gcolr->val.general.y,
                       gcolr->val.general.z,
                       gcolr->val.general.a);
    } else {
      glColor4f(gcolr->val.general.x,
                gcolr->val.general.y,
                gcolr->val.general.z,
                gcolr->val.general.a);
    }
    break;

  default:
    break;
  }
}

/*******************************************************************************
 * wsgl_convert_gcolr
 *
 * DESCR:      convert gcolr to target model
 * RETURNS:    N/A
 */
void wsgl_convert_gcolr(
                        Pgcolr *gcolr,
                        int model
                        ){
  switch (gcolr->type){
  case PINDIRECT:
    break;
  case PMODEL_RGB:
    if (model == PMODEL_RGBA){
      gcolr->val.general.a = 1.0;
      gcolr->type = PMODEL_RGBA;
    }
    break;
  case PMODEL_RGBA:
    if (model == PMODEL_RGB) gcolr->type = PMODEL_RGB;
    break;
  default:
    break;
  }
}

/*******************************************************************************
 * wsgl_colr_from_gcolr
 *
 * DESCR:    Get colour value from Pgcolr
 * RETURNS:    N/A
 */
void wsgl_colr_from_gcolr(
                          Pcoval *colr,
                          Pgcolr *gcolr,
                          Pint mode
                          )
{
  /* mode is the global color mode */


  colr->direct.rgba.alpha = 1.0;

  /* check the type of the given color */
  switch(gcolr->type) {
  case PINDIRECT:
    colr->ind = gcolr->val.ind;
    break;

  case PMODEL_RGB:
    if (mode == PMODEL_RGB){
      colr->direct.rgb.red = gcolr->val.general.x;
      colr->direct.rgb.green = gcolr->val.general.y;
      colr->direct.rgb.blue = gcolr->val.general.z;
    } else {
      /* have RGB but want rbga */
      colr->direct.rgba.red = gcolr->val.general.x;
      colr->direct.rgba.green = gcolr->val.general.y;
      colr->direct.rgba.blue = gcolr->val.general.z;
      colr->direct.rgba.alpha = 1.0;
    }
    break;

  case PMODEL_RGBA:
    if (mode == PMODEL_RGBA){
      colr->direct.rgba.red = gcolr->val.general.x;
      colr->direct.rgba.green = gcolr->val.general.y;
      colr->direct.rgba.blue = gcolr->val.general.z;
      colr->direct.rgba.alpha = gcolr->val.general.a;
    } else {
      /* have RGBA but want RGB */
      colr->direct.rgb.red = gcolr->val.general.x;
      colr->direct.rgb.green = gcolr->val.general.y;
      colr->direct.rgb.blue = gcolr->val.general.z;
    }
    break;

  default:
    break;
  }
}

/*******************************************************************************
 * wsgl_set_line_ind
 *
 * DESCR:    Setup line index
 * RETURNS:    N/A
 */
void wsgl_set_line_ind(
                       Ws *ws,
                       Pattr_group *attr_group,
                       Pint ind
                       )
{
  Phg_ret ret;

  (*ws->inq_representation)(ws,
                            ind,
                            PINQ_REALIZED,
                            PHG_ARGS_EXTLNREP,
                            &ret);
  if (ret.err == 0) {
    phg_attr_group_set_line_bundle(ws,
                                   attr_group,
                                   &ret.data.rep.extlnrep);
  }
}

/*******************************************************************************
 * wsgl_setup_line_attr
 *
 * DESCR:    Setup line attributes
 * RETURNS:    N/A
 */
void wsgl_setup_line_attr(
                          Ws_attr_st *ast
                          )
{
  Pint type;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_LINE_COLR_IND)) {
    wsgl_set_gcolr(&ast->indiv_group.line_bundle.colr);
  }
  else {
    wsgl_set_gcolr(&ast->bundl_group.line_bundle.colr);
  }

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_LINETYPE)) {
    type = ast->indiv_group.line_bundle.type;
  }
  else {
    type = ast->bundl_group.line_bundle.type;
  }

  switch (type) {
  case PLINE_DASH:
    glLineStipple(1, 0x00ff);
    glEnable(GL_LINE_STIPPLE);
    break;

  case PLINE_DOT:
    glLineStipple(1, 0x0101);
    glEnable(GL_LINE_STIPPLE);
    break;

  case PLINE_DASH_DOT:
    glLineStipple(1, 0x1c47);
    glEnable(GL_LINE_STIPPLE);
    break;

  default:
    glDisable(GL_LINE_STIPPLE);
    break;
  }
  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_LINEWIDTH)) {
    glLineWidth(ast->indiv_group.line_bundle.width);
  }
  else {
    glLineWidth(ast->bundl_group.line_bundle.width);
  }
  if (wsgl_use_shaders){
    glEnable(GL_LINE_SMOOTH);
    glUniform1i(shading_mode, 0);
  } else {
    glDisable(GL_LIGHTING);
  }
}

/*******************************************************************************
 * wsgl_set_int_ind
 *
 * DESCR:    Setup interior index
 * RETURNS:    N/A
 */
void wsgl_set_int_ind(
                      Ws *ws,
                      Pattr_group *attr_group,
                      Pint ind
                      )
{
  Phg_ret ret;

  (*ws->inq_representation)(ws,
                            ind,
                            PINQ_REALIZED,
                            PHG_ARGS_EXTINTERREP,
                            &ret);
  if (ret.err == 0) {
    phg_attr_group_set_int_bundle(ws,
                                  attr_group,
                                  &ret.data.rep.extinterrep);
  }
}

/*******************************************************************************
 * wsgl_get_int_colr
 *
 * DESCR:    Get interior colour
 * RETURNS:    Pointer to interiour colour
 */
Pgcolr* wsgl_get_int_colr(
                          Ws_attr_st *ast
                          )
{
  Pgcolr *gcolr;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_INT_COLR_IND)) {
    gcolr = &ast->indiv_group.int_bundle.colr;
#ifdef DEBUGA
    printf("gcolr from nameset %d colors %f %f %f %f\n",
           gcolr->type,
           gcolr->val.general.x,
           gcolr->val.general.y,
           gcolr->val.general.z,
           gcolr->val.general.a
           );
#endif
  }
  else {
    gcolr = &ast->bundl_group.int_bundle.colr;
#ifdef DEBUGA
    printf("gcolr from bundle %d colors %f %f %f %f\n",
           gcolr->type,
           gcolr->val.general.x,
           gcolr->val.general.y,
           gcolr->val.general.z,
           gcolr->val.general.a
           );
#endif
  }
  return gcolr;
}

/*******************************************************************************
 * wsgl_get_int_style
 *
 * DESCR:    Get interior style
 * RETURNS:    Interiour style
 */
Pint_style wsgl_get_int_style(
                              Ws_attr_st *ast
                              )
{
  Pint_style style;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_INT_STYLE)) {
    style = ast->indiv_group.int_bundle.style;
  }
  else {
    style = ast->bundl_group.int_bundle.style;
  }

  return style;
}

/*******************************************************************************
 * wsgl_setup_int_style
 *
 * DESCR:    Setup interior style
 * RETURNS:    N/A
 */
void wsgl_setup_int_style(
                          Pint_style style
                          )
{
  switch (style) {
  case PSTYLE_HOLLOW:
    glDisable(GL_POLYGON_STIPPLE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;

  case PSTYLE_SOLID:
    glDisable(GL_POLYGON_STIPPLE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;

  case PSTYLE_HATCH:
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;

  default:
    glDisable(GL_POLYGON_STIPPLE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  }
}

/*******************************************************************************
 * wsgl_setup_int_attr_nocol
 *
 * DESCR:    Setup interior attributes without color
 * RETURNS:    N/A
 */
void wsgl_setup_int_attr_nocol(
                               Ws *ws,
                               Ws_attr_st *ast
                               )
{
  Pint_style style;
  Pint style_ind;
  Pint shad_meth;

  Wsgl_handle wsgl = ws->render_context;

  style = wsgl_get_int_style(ast);
  if (style != wsgl->dev_st.int_style) {
    wsgl_setup_int_style(style);
    wsgl->dev_st.int_style = style;
  }

  if (phg_nset_name_is_set(&ast->asf_nameset,
                           (Pint) PASPECT_INT_STYLE_IND)) {
    style_ind = ast->indiv_group.int_bundle.style_ind;
  }
  else {
    style_ind = ast->bundl_group.int_bundle.style_ind;
  }

  if (style_ind != wsgl->dev_st.int_style_ind) {
    glPolygonStipple(wsgl_hatch_tbl[style_ind - 1]);
    wsgl->dev_st.int_style_ind = style_ind;
  }

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_INT_SHAD_METH)) {
    shad_meth = ast->indiv_group.int_bundle.shad_meth;
  }
  else {
    shad_meth = ast->bundl_group.int_bundle.shad_meth;
  }

  if (shad_meth != wsgl->dev_st.int_shad_meth) {
    if (shad_meth == PSD_NONE) {
      glShadeModel(GL_FLAT);
    }
    else {
      glShadeModel(GL_SMOOTH);
    }
    wsgl->dev_st.int_shad_meth = shad_meth;
  }

  if (wsgl_use_shaders){
    if (wsgl->cur_struct.lighting) {
      glUniform1i(shading_mode, 1);
    }
    else {
      glUniform1i(shading_mode, 0);
    }
  } else {
    if (wsgl->cur_struct.lighting) {
      glEnable(GL_LIGHTING);
    }
    else {
      glDisable(GL_LIGHTING);
    }
  }
  glCullFace(GL_BACK);
}

/*******************************************************************************
 * wsgl_setup_int_attr
 *
 * DESCR:    Setup interior attributes
 * RETURNS:    N/A
 */
void wsgl_setup_int_attr(
                         Ws *ws,
                         Ws_attr_st *ast
                         )
{
  wsgl_set_gcolr(wsgl_get_int_colr(ast));
  wsgl_setup_int_attr_nocol(ws, ast);
}

/*******************************************************************************
 * wsgl_set_edge_ind
 *
 * DESCR:    Setup edge index
 * RETURNS:    N/A
 */
void wsgl_set_edge_ind(
                       Ws *ws,
                       Pattr_group *attr_group,
                       Pint ind
                       )
{
  Phg_ret ret;

  (*ws->inq_representation)(ws,
                            ind,
                            PINQ_REALIZED,
                            PHG_ARGS_EXTEDGEREP,
                            &ret);
  if (ret.err == 0) {
    phg_attr_group_set_edge_bundle(ws,
                                   attr_group,
                                   &ret.data.rep.extedgerep);
  }
}

/*******************************************************************************
 * wsgl_get_edge_flag
 *
 * DESCR:    Get edge flag
 * RETURNS:    Edge flag
 */
Pedge_flag wsgl_get_edge_flag(
                              Ws_attr_st *ast
                              )
{
  Pedge_flag flag;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGE_FLAG)) {
    flag = ast->indiv_group.edge_bundle.flag;
  }
  else {
    flag = ast->bundl_group.edge_bundle.flag;
  }
  return flag;
}

/*******************************************************************************
 * wsgl_get_edge_width
 *
 * DESCR:    Get edge width
 * RETURNS:    Edge width
 */
Pfloat wsgl_get_edge_width(
                           Ws_attr_st *ast
                           )
{
  Pfloat width;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGEWIDTH)) {
    width = ast->indiv_group.edge_bundle.width;
  }
  else {
    width = ast->bundl_group.edge_bundle.width;
  }

  return width;
}

/*******************************************************************************
 * wsgl_setup_edge_attr
 *
 * DESCR:    Setup edge attributes
 * RETURNS:    N/A
 */
void wsgl_setup_edge_attr(
                          Ws_attr_st *ast
                          )
{
  Pint type;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGE_COLR_IND)) {
    wsgl_set_gcolr(&ast->indiv_group.edge_bundle.colr);
  }
  else {
    wsgl_set_gcolr(&ast->bundl_group.edge_bundle.colr);
  }

  glLineWidth(wsgl_get_edge_width(ast));

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_EDGETYPE)) {
    type = ast->indiv_group.edge_bundle.type;
  }
  else {
    type = ast->bundl_group.edge_bundle.type;
  }

  /* Line style */
  switch (type) {
  case PLINE_DASH:
    glLineStipple(1, 0x00ff);
    glEnable(GL_LINE_STIPPLE);
    break;

  case PLINE_DOT:
    glLineStipple(1, 0x0101);
    glEnable(GL_LINE_STIPPLE);
    break;

  case PLINE_DASH_DOT:
    glLineStipple(1, 0x1c47);
    glEnable(GL_LINE_STIPPLE);
    break;

  default:
    glDisable(GL_LINE_STIPPLE);
    break;
  }
  if (wsgl_use_shaders) {
    glUniform1i(shading_mode, 0);
  } else {
    glDisable(GL_LIGHTING);
  }
}

/*******************************************************************************
 * wsgl_set_marker_ind
 *
 * DESCR:    Setup marker index
 * RETURNS:    N/A
 */
void wsgl_set_marker_ind(
                         Ws *ws,
                         Pattr_group *attr_group,
                         Pint ind
                         )
{
  Phg_ret ret;

  (*ws->inq_representation)(ws,
                            ind,
                            PINQ_REALIZED,
                            PHG_ARGS_EXTMKREP,
                            &ret);
  if (ret.err == 0) {
    phg_attr_group_set_marker_bundle(ws,
                                     attr_group,
                                     &ret.data.rep.extmkrep);
  }
}

/*******************************************************************************
 * wsgl_setup_marker_attr
 *
 * DESCR:    Setup marker attributes
 * RETURNS:    N/A
 */
void wsgl_setup_marker_attr(
                            Ws_attr_st *ast,
                            Pint *type,
                            Pfloat *size
                            )
{
  if (phg_nset_name_is_set(&ast->asf_nameset,
                           (Pint) PASPECT_MARKER_COLR_IND)) {
    wsgl_set_gcolr(&ast->indiv_group.marker_bundle.colr);
  }
  else {
    wsgl_set_gcolr(&ast->bundl_group.marker_bundle.colr);
  }

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_MARKER_TYPE)) {
    *type = ast->indiv_group.marker_bundle.type;
  }
  else {
    *type = ast->bundl_group.marker_bundle.type;
  }

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_MARKER_SIZE)) {
    *size = ast->indiv_group.marker_bundle.size;
  }
  else {
    *size = ast->bundl_group.marker_bundle.size;
  }
  if (wsgl_use_shaders){
    glUniform1i(shading_mode, 0);
  } else {
    glDisable(GL_LIGHTING);
  }
}

/*******************************************************************************
 * wsgl_setup_background
 *
 * DESCR:    Setup background colour
 * RETURNS:    N/A
 */
void wsgl_setup_background(
                           Ws *ws
                           )
{
  Wsgl_handle wsgl = ws->render_context;
  glDisable(GL_POLYGON_STIPPLE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (wsgl_use_shaders) {
    glVertexAttrib4f(vCOLOR,
                     wsgl->background.val.general.x,
                     wsgl->background.val.general.y,
                     wsgl->background.val.general.z,
                     wsgl->background.val.general.a);
  } else {
    if (ws->current_colour_model == PMODEL_RGBA){
      glColor4f(wsgl->background.val.general.x,
                wsgl->background.val.general.y,
                wsgl->background.val.general.z,
                wsgl->background.val.general.a
                );
    } else {
      glColor4f(wsgl->background.val.general.x,
                wsgl->background.val.general.y,
                wsgl->background.val.general.z,
                1.0
                );
    }
  }
  /* Need to restore polygon mode */
  wsgl->dev_st.int_style = -1;
}

/*******************************************************************************
 * wsgl_set_text_ind
 *
 * DESCR:    Setup text index
 * RETURNS:    N/A
 */
void wsgl_set_text_ind(
                       Ws *ws,
                       Pattr_group *attr_group,
                       Pint ind
                       )
{
  Phg_ret ret;

  (*ws->inq_representation)(ws,
                            ind,
                            PINQ_REALIZED,
                            PHG_ARGS_EXTTXREP,
                            &ret);
  if (ret.err == 0) {
    phg_attr_group_set_text_bundle(ws,
                                   attr_group,
                                   &ret.data.rep.exttxrep);
  }
}

/*******************************************************************************
 * wsgl_get_text_prec
 *
 * DESCR:    Get text precision
 * RETURNS:    Text precision
 */
Ptext_prec wsgl_get_text_prec(
                              Ws_attr_st *ast
                              )
{
  Ptext_prec prec;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_TEXT_PREC)) {
    prec = ast->indiv_group.text_bundle.prec;
  }
  else {
    prec = ast->bundl_group.text_bundle.prec;
  }

  return prec;
}

/*******************************************************************************
 * wsgl_setup_text_attr
 *
 * DESCR:    Setup text attributes
 * RETURNS:    N/A
 */

void wsgl_setup_text_attr(
                          Ws_attr_st *ast,
                          Phg_font **fnt,
                          Pfloat *char_expan
                          )
{
  Pint font;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_TEXT_COLR_IND)) {
    wsgl_set_gcolr(&ast->indiv_group.text_bundle.colr);
  }
  else {
    wsgl_set_gcolr(&ast->bundl_group.text_bundle.colr);
  }

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_TEXT_FONT)) {
    font = ast->indiv_group.text_bundle.font;
  }
  else {
    font = ast->bundl_group.text_bundle.font;
  }

  if (font < 1) {
    *fnt = fnt_fonts[1];
  }
  else {
    *fnt = fnt_fonts[font - 1];
  }

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_CHAR_EXPAN)) {
    *char_expan = ast->indiv_group.text_bundle.char_expan;
  }
  else {
    *char_expan = ast->bundl_group.text_bundle.char_expan;
  }

  if (wsgl_use_shaders){
    glUniform1i(shading_mode, 0);
  }
  else
    {
      glDisable(GL_LIGHTING);
      glLineWidth(2.0);
    }
}

/*******************************************************************************
 * wsgl_get_char_space
 *
 * DESCR:    Get char spacing
 * RETURNS:    Character spacing
 */
Pfloat wsgl_get_char_space(
                           Ws_attr_st *ast
                           )
{
  Pfloat char_space;

  if (phg_nset_name_is_set(&ast->asf_nameset, (Pint) PASPECT_CHAR_SPACE)) {
    char_space = ast->indiv_group.text_bundle.char_space;
  }
  else {
    char_space = ast->bundl_group.text_bundle.char_space;
  }

  return char_space;
}

/*******************************************************************************
 * wsgl_add_names_set
 *
 * DESCR:    Add names to nameset
 * RETURNS:    N/A
 */
void wsgl_add_names_set(
                        Ws *ws,
                        void *names
                        )
{
  Pint num_ints;
  Pint *data = (Pint *) names;
  Wsgl_handle wsgl = ws->render_context;

  num_ints = *data;
  data++;

  phg_nset_names_set(&wsgl->cur_struct.cur_nameset,
                     num_ints,
                     data);
}

/*******************************************************************************
 * wsgl_remove_names_set
 *
 * DESCR:    Remove names from nameset
 * RETURNS:  N/A
 */
void wsgl_remove_names_set(
                           Ws *ws,
                           void *names
                           )
{
  Pint num_ints;
  Pint *data = (Pint *) names;
  Wsgl_handle wsgl = ws->render_context;

  num_ints = *data;
  data++;

  phg_nset_names_clear(&wsgl->cur_struct.cur_nameset,
                       num_ints,
                       data);
}
