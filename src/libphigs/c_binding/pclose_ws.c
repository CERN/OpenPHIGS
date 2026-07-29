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
#include <png.h>
#ifdef GLEW
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif
#include <gl2ps.h>

#include "phg.h"
#include "css.h"
#include "ws.h"
#include "private/phgP.h"
#include "private/cbP.h"
#include "private/wsglP.h"
#include "private/wsxP.h"
#include "phconf.h"

short int wsgl_use_shaders_settings;

/* Automatically generated prototypes */
extern void popen_ws(
              Pint ws_id,
              Phg_args_conn_info *conn_id,
              Pint ws_type
              );
extern void pclose_ws(
               Pint ws_id
               );
extern void pset_ws_vp(
                Pint ws_id,
                Plimit *viewport
                );
extern void pset_ws_vp3(
                 Pint ws_id,
                 Plimit3 *viewport
                 );
extern void pset_ws_win(
                 Pint ws_id,
                 Plimit *window
                 );
extern void pset_ws_win3(
                  Pint ws_id,
                  Plimit3 *window
                  );
extern void pset_invis_filter(
                       Pint ws_id,
                       Pfilter *filter
                       );
extern void pset_hlhsr_mode(
                     Pint ws_id,
                     Pint hlhsr_mode
                     );
extern void pset_line_rep(
                   Pint ws_id,
                   Pint line_ind,
                   Pline_bundle *line_bundle
                   );
extern void pset_marker_rep(
                     Pint ws_id,
                     Pint marker_ind,
                     Pmarker_bundle *marker_bundle
                     );
extern void pset_text_rep(
                   Pint ws_id,
                   Pint text_ind,
                   Ptext_bundle *text_bundle
                   );
extern void pset_edge_rep(
                   Pint ws_id,
                   Pint edge_ind,
                   Pedge_bundle *edge_bundle
                   );
extern void pset_int_rep(
                  Pint ws_id,
                  Pint int_ind,
                  Pint_bundle *int_bundle
                  );
extern void ppost_struct(
                  Pint ws_id,
                  Pint struct_id,
                  Pfloat priority
                  );
extern void punpost_struct(
                    Pint ws_id,
                    Pint struct_id
                    );
extern void punpost_all_structs(
                         Pint ws_id
                         );
extern void predraw_all_structs(
                         Pint ws_id,
                         Pctrl_flag ctrl_flag
                         );
extern void pupd_ws(
             Pint ws_id,
             Pregen_flag regen_flag
             );
extern void pset_disp_upd_st(
                      Pint ws_id,
                      Pdefer_mode def_mode,
                      Pmod_mode mod_mode
                      );
extern void pset_view_tran_in_pri(
                           Pint ws_id,
                           Pint view_ind,
                           Pint ref_view_ind,
                           Prel_pri rel_pri
                           );
extern void pset_light_src_rep(
                        Pint ws_id,
                        Pint light_src_ind,
                        Plight_src_bundle *light_src_rep
                        );
extern void pset_colr_model(
                     Pint ws_id,
                     Pint model
                     );
extern void pset_colr_rep(
                   Pint ws_id,
                   Pint ind,
                   Pcolr_rep *rep
                   );
extern void pinq_ws_st(
                Pws_st *ws_state
                );
extern void pinq_ws_conn_type(
                       Pint ws_id,
                       Pstore store,
                       Pint *err_ind,
                       void **conn_id,
                       Pint *ws_type
                       );
extern void pinq_open_wss(
                   Pint num_elems_appl_list,
                   Pint start_ind,
                   Pint *err_ind,
                   Pint_list *open_ws_ids,
                   Pint *num_elems_impl_list
                   );
extern void pinq_table_indices(
                               Phg_args_rep_type type,
                               Pint ws_id,
                               Pint num_elems_appl_list,
                               Pint start_ind,
                               Pint *err_ind,
                               Pint_list *def_line_ind,
                               Pint *num_elems_impl_list
                               );
extern void pinq_list_view_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *view_inds,
                         Pint *num_elems_impl_list
                         );
extern void pinq_list_line_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *def_line_ind,
                         Pint *num_elems_impl_list
                         );
extern void pinq_list_marker_inds(
                           Pint ws_id,
                           Pint num_elems_appl_list,
                           Pint start_ind,
                           Pint *err_ind,
                           Pint_list *def_marker_ind,
                           Pint *num_elems_impl_list
                           );
extern void pinq_list_text_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *def_text_ind,
                         Pint *num_elems_impl_list
                         );
extern void pinq_list_int_inds(
                        Pint ws_id,
                        Pint num_elems_appl_list,
                        Pint start_ind,
                        Pint *err_ind,
                        Pint_list *def_int_ind,
                        Pint *num_elems_impl_list
                        );
extern void pinq_list_edge_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *def_edge_ind,
                         Pint *num_elems_impl_list
                         );
extern void pinq_list_colr_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *colr_ind,
                         Pint *num_elems_impl_list
                         );
extern void pinq_line_rep(
                   Pint ws_id,
                   Pint index,
                   Pinq_type type,
                   Pint *err_ind,
                   Pline_bundle *line_rep
                   );
extern void pinq_marker_rep(
                     Pint ws_id,
                     Pint index,
                     Pinq_type type,
                     Pint *err_ind,
                     Pmarker_bundle *marker_rep
                     );
extern void pinq_text_rep(
                   Pint ws_id,
                   Pint index,
                   Pinq_type type,
                   Pint *err_ind,
                   Ptext_bundle *text_rep
                   );
extern void pinq_int_rep(
                  Pint ws_id,
                  Pint index,
                  Pinq_type type,
                  Pint *err_ind,
                  Pint_bundle *int_rep
                  );
extern void pinq_edge_rep(
                   Pint ws_id,
                   Pint index,
                   Pinq_type type,
                   Pint *err_ind,
                   Pedge_bundle *edge_rep
                   );
extern void pinq_colr_rep(
                   Pint ws_id,
                   Pint colr_ind,
                   Pinq_type type,
                   Pint *err_ind,
                   Pcolr_rep *colr_rep
                   );
extern void inq_filter(
                       Phg_args_flt_type type,
                       Pint ws_id,
                       struct _Pstore *store,
                       Pint *err_ind,
                       Pfilter **filter
                       );
extern void pinq_invis_filter(
                       Pint ws_id,
                       Pstore store,
                       Pint *err_ind,
                       Pfilter **invis_filter
                       );
extern void pmessage(
              Pint ws_id,
              char* message
              );


/**
 * \file pclose_ws.c
 * \brief Close workstation
 */
void pclose_ws(
               Pint ws_id
               )
{
  Ws_handle wsh;
  Wsb_output_ws *owsb;
  Ws_post_str *str;
  Wst_phigs_dt *dt;
  Psl_ws_info *wsinfo;
  int width, height;
  unsigned int buffer_size;
  int error;
  int channels;
  int nvals;
  int i;
  int clean_fb = FALSE;
  int gl2ps = 0;
  int ctrl_flag = 0;
  GLubyte * pixel_buffer;
  png_byte ** png_rows;
  png_structp png;

  if (phg_ws_open(ws_id, Pfn_close_ws) != NULL) {
    wsh = PHG_WSID(ws_id);
    int width = wsh->type->desc_tbl.xwin_dt.tool.width;
    int height = wsh->type->desc_tbl.xwin_dt.tool.height;
    wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
    dt = &wsinfo->wstype->desc_tbl.phigs_dt;
    glFlush();
    glFinish();
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    switch (dt->ws_category){
    case PCAT_IN:
    case PCAT_OUT:
    case PCAT_OUTIN:
    case PCAT_MO:
    case PCAT_MI:
      break;
    case PCAT_TGA:
      buffer_size = 3*width*height*sizeof(GLubyte);
      pixel_buffer = (GLubyte * ) malloc(buffer_size);
      glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixel_buffer);
      error = glGetError();
      if (error != GL_NO_ERROR ){
        printf("PCLOSEWS ERROR: glReadPixel returned error code %d\n", error);
      }
      short header[] = {0, 2, 0, 0, 0, 0, (short) width, (short) height, 24};
      wsh->fd = fopen(wsh->filename, "w+");
      fwrite(&header, sizeof(header), 1, wsh->fd);
      fwrite(pixel_buffer, buffer_size, 1, wsh->fd);
      fclose(wsh->fd);
      free(pixel_buffer);
      clean_fb = TRUE;
      break;
    case PCAT_PNG:
      png_rows = (png_byte**)malloc(height * sizeof(png_byte*));
      channels = 3;
      buffer_size = channels*width*height*sizeof(GLubyte);
      pixel_buffer = (GLubyte*) malloc(buffer_size);
      nvals = channels * width * height;
      glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel_buffer);
      error = glGetError();
      if (error != GL_NO_ERROR ){
        printf("PCLOSEWS ERROR: glReadPixel returned error code %d\n", error);
      }
      for (i=0; i<height; i++){
        png_rows[i] = &(pixel_buffer[ (height - i - 1) * width * channels]);
      }
      png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
      if (png) {
        png_infop info = png_create_info_struct(png);
        if (info){
          wsh->fd = fopen(wsh->filename, "w+");
          setjmp(png_jmpbuf(png));
          png_init_io(png, wsh->fd);
          png_set_IHDR(
                       png,
                       info,
                       width, height,
                       8,
                       PNG_COLOR_TYPE_RGB,
                       PNG_INTERLACE_NONE,
                       PNG_COMPRESSION_TYPE_DEFAULT,
                       PNG_FILTER_TYPE_DEFAULT
                       );
          png_write_info(png, info);
          png_write_image(png, png_rows);
          png_write_end(png, NULL);
          fclose(wsh->fd);
        } else {
          printf("PNG export error: failed to create info structure\n");
        }
        png_destroy_write_struct(&png, &info);
      } else {
        printf("PNG export error: failed to create write structure\n");
      }
      free(pixel_buffer);
      free(png_rows);
      clean_fb = TRUE;
      break;
    case PCAT_PNGA:
      channels = 4;
      png_rows = (png_byte**)malloc(height * sizeof(png_byte*));
      buffer_size = channels*width*height*sizeof(GLubyte);
      pixel_buffer = (GLubyte*) malloc(buffer_size);
      nvals = channels * width * height;
      glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixel_buffer);
      error = glGetError();
      if (error != GL_NO_ERROR ){
        printf("PCLOSEWS ERROR: glReadPixel returned error code %d\n", error);
      }
      for (i=0; i<height; i++){
        png_rows[i] = &(pixel_buffer[ (height - i - 1) * width * channels]);
      }
      png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
      if (png) {
        png_infop info = png_create_info_struct(png);
        if (info){
          wsh->fd = fopen(wsh->filename, "w+");
          setjmp(png_jmpbuf(png));
          png_init_io(png, wsh->fd);
          png_set_IHDR(
                       png,
                       info,
                       width, height,
                       8,
                       PNG_COLOR_TYPE_RGBA,
                       PNG_INTERLACE_NONE,
                       PNG_COMPRESSION_TYPE_DEFAULT,
                       PNG_FILTER_TYPE_DEFAULT
                       );
          png_write_info(png, info);
          png_write_image(png, png_rows);
          png_write_end(png, NULL);
          fclose(wsh->fd);
        } else {
          printf("PNG export error: failed to create info structure\n");
        }
        png_destroy_write_struct(&png, &info);
      } else {
        printf("PNG export error: failed to create write structure\n");
      }
      free(pixel_buffer);
      free(png_rows);
      clean_fb = TRUE;
      break;
    case PCAT_EPS:
      gl2ps = GL2PS_EPS;
      break;
    case PCAT_PDF:
      gl2ps = GL2PS_PDF;
      break;
    case PCAT_SVG:
      gl2ps = GL2PS_SVG;
      break;
    case PCAT_OBJ:
      wsgl_export_obj(wsh->filename, config[ws_id].window_title);
      record_geom = FALSE;
      wsgl_clear_geometry();
      clean_fb = TRUE;
      break;
    default:
      break;
    }
    if (gl2ps){
      int buffsize = 1024*1024*1024;
      wsh->fd = fopen(wsh->filename, "wb");
      gl2psBeginPage(config[ws_id].window_title, "OpenPHIGS", NULL, gl2ps, GL2PS_BSP_SORT,
                     GL2PS_DRAW_BACKGROUND | GL2PS_USE_CURRENT_VIEWPORT| GL2PS_SIMPLE_LINE_OFFSET|GL2PS_OCCLUSION_CULL|GL2PS_TIGHT_BOUNDING_BOX,
                     GL_RGBA, 0, NULL, 0, 0, 0, buffsize, wsh->fd, NULL);
      /* redefine colors and redraw */
      ctrl_flag = 0;
      pxset_color_map(ws_id);
      if (wsh->current_colour_model == PMODEL_RGBA){
        pset_colr_rep(ws_id, 0, &(config[ws_id].background_color_rgba));
      } else {
        pset_colr_rep(ws_id, 0, &(config[ws_id].background_color_rgb));
      }
      predraw_all_structs(ws_id, ctrl_flag);
      int state = gl2psEndPage();
      if (state == GL2PS_OVERFLOW) {
        printf("ERROR exporting as eps. Buffer size is too small.\n");
      }
      fclose(wsh->fd);
      /* restore original shader settings */
      wsgl_use_shaders = wsgl_use_shaders_settings;
      clean_fb = TRUE;
    }
    (*wsh->update)(wsh, PFLAG_PERFORM);
    owsb = &wsh->out_ws.model.b;
    str = owsb->posted.lowest.higher;
    while (str->higher != NULL) {
      phg_css_unpost(owsb->cssh, str->structh->struct_id, wsh);
      str = str->higher;
    }
    /* cleanup */
    if (wsh->glx_context){
      glXDestroyContext(wsh->display, wsh->glx_context);
    }
    if (clean_fb){
      phg_wsx_cleanup_fb(wsh);
#ifdef DEBUG
      printf("Restoring view port %d %d %d %d",
             wsh->old_viewport[0],
             wsh->old_viewport[1],
             wsh->old_viewport[2],
             wsh->old_viewport[3]);
#endif
      glViewport(wsh->old_viewport[0],
                 wsh->old_viewport[1],
                 wsh->old_viewport[2],
                 wsh->old_viewport[3]);
    }
    (*wsh->close)(wsh);
    phg_psl_rem_ws(PHG_PSL, ws_id);

  } else {
    printf("PCLOSEWS ERROR: workstation was not open. Ignoring function.");
  }
}

