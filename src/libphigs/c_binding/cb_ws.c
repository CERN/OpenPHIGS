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

/*******************************************************************************
 * popen_ws
 *
 * DESCR:	Open workstation
 * RETURNS:	N/A
 */

void popen_ws(
              Pint ws_id,
              Phg_args_conn_info *conn_id,
              Pint ws_type
              )
{
  Wst *wst;
  Ws_handle wsh;
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Phg_args_open_ws args;
  Phg_ret ret;
  Pcolr_rep rep;
  char* filename;
  /* read default configuration file if not read yet */
  if (! config_read){
    config_read = 1;
    read_config("phigs.def");
  };
  /* save the current shader settings */
  printf("popen_ws: %d %d\n", wsgl_use_shaders, wsgl_use_shaders_settings);
  wsgl_use_shaders_settings = wsgl_use_shaders;
  if (phg_entry_check(PHG_ERH, ERR2, Pfn_open_ws)) {
    if ((ws_id < 0) || (ws_id > MAX_NO_OPEN_WS)) {
      ERR_REPORT(PHG_ERH, ERR65);
    }
    else if (phg_psl_inq_ws_open(PHG_PSL, ws_id)) {
      ERR_REPORT(PHG_ERH, ERR53);
    }
    else if (!phg_psl_ws_free_slot(PHG_PSL)) {
      ERR_REPORT(PHG_ERH, ERR63);
    }
    else {
      wst = phg_wst_find(&PHG_WST_LIST, ws_type);

      if (wst == NULL) {
        ERR_REPORT(PHG_ERH, ERR52);
      }
      else {
        memset(&args, 0, sizeof(Phg_args_open_ws));
        args.width = config[ws_id].display_width;
        args.height = config[ws_id].display_height;
        args.hcsf = config[ws_id].hcsf;
#ifdef DEBUG
        printf("cb_ws: WSID=%d type=%d scale factor %f\n", ws_id, ws_type, args.hcsf);
#endif
        if (conn_id == NULL) {
          args.conn_type = PHG_ARGS_CONN_OPEN;
        }
        else {
          args.conn_info.background = 0;
          if (
              ws_type == PWST_HCOPY_TRUE_TGA ||
              ws_type == PWST_HCOPY_TRUE_RGB_PNG ||
              ws_type == PWST_HCOPY_TRUE_RGBA_PNG ||
              ws_type == PWST_HCOPY_TRUE_EPS ||
              ws_type == PWST_HCOPY_TRUE_PDF ||
              ws_type == PWST_HCOPY_TRUE_SVG
              ) {
            args.conn_type = PHG_ARGS_CONN_HCOPY;
            args.width = config[ws_id].display_width*config[ws_id].hcsf;
            args.height = config[ws_id].display_height*config[ws_id].hcsf;
            /* color index zero is background */
            memcpy(&args.conn_info, conn_id, sizeof(Phg_args_conn_info));
          }
          else {
            args.conn_type = PHG_ARGS_CONN_DRAWABLE;
            memcpy(&args.conn_info, conn_id, sizeof(Phg_args_conn_info));
          }
        }
        /* switch off shaders for EPS export */
        if (ws_type == PWST_HCOPY_TRUE_EPS  || ws_type == PWST_HCOPY_TRUE_PDF || ws_type == PWST_HCOPY_TRUE_SVG ){
          //          printf("popen_ws: switch off shaders\n");
          wsgl_use_shaders_settings = wsgl_use_shaders;
          wsgl_use_shaders = 0;
        }
        args.wsid = ws_id;
        args.type = wst;
        args.erh = PHG_ERH;
        args.cssh = PHG_CSS;
        args.memory = 8192;
        args.input_q = PHG_INPUT_Q;
        args.window_name = config[ws_id].window_title;
        args.icon_name = config[ws_id].window_icon;
        args.x = config[ws_id].xpos;
        args.y = config[ws_id].ypos;
        args.border_width =  config[ws_id].border_width;
        args.limits = config[ws_id].vpos;

        /* Open workstation */
        PHG_WSID(ws_id) = (*wst->desc_tbl.phigs_dt.ws_open)(&args, &ret);
        if (PHG_WSID(ws_id) == NULL) {
          ERR_REPORT(PHG_ERH, ERR900);
        }
        else {
          /* Add workstation to info list */
          phg_psl_add_ws(PHG_PSL, ws_id, NULL, wst);
        }
        /* predefine some colors */
        pxset_color_map(ws_id);
        /* set background as specified in configuration file */
        pset_colr_rep(ws_id, 0, &(config[ws_id].background_color));
        wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
        dt = &wsinfo->wstype->desc_tbl.phigs_dt;
        /* init the file name */
        wsh = PHG_WSID(ws_id);
        if (strlen(config[ws_id].filename) == 0){
          switch (dt->ws_category){
          case PCAT_TGA:
            strcpy(wsh->filename, "output.tga");
            break;
          case PCAT_PNG:
          case PCAT_PNGA:
            strcpy(wsh->filename, "output.png");
            break;
          case PCAT_EPS:
            strcpy(wsh->filename, "output.eps");
            break;
          case PCAT_PDF:
            strcpy(wsh->filename, "output.pdf");
            break;
          case PCAT_SVG:
            strcpy(wsh->filename, "output.svg");
            break;
          case PCAT_IN:
          case PCAT_OUT:
          case PCAT_OUTIN:
          case PCAT_MO:
          case PCAT_MI:
            break;
          default:
            break;
          }
        } else {
          strncpy(wsh->filename, config[ws_id].filename, sizeof(wsh->filename));
        }
        wsgl_clear(wsh);
      }
    }
    ERR_FLUSH(PHG_ERH);
  }
}

/*******************************************************************************
 * pclose_ws
 *
 * DESCR:	Close workstation
 * RETURNS:	N/A
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
  GLubyte * pixel_buffer;
  png_byte ** png_rows;
  png_structp png;
  int clean_fb = FALSE;
  int gl2ps = 0;
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
    default:
      break;
    }
    if (gl2ps){
      int buffsize = 1024*1024*1024;
      wsh->fd = fopen(wsh->filename, "wb");
      gl2psBeginPage(config[ws_id].window_title, "OpenPHIGS", NULL, gl2ps, GL2PS_BSP_SORT,
                     GL2PS_DRAW_BACKGROUND | GL2PS_USE_CURRENT_VIEWPORT| GL2PS_SIMPLE_LINE_OFFSET|GL2PS_OCCLUSION_CULL|GL2PS_TIGHT_BOUNDING_BOX,
                     GL_RGBA, 0, NULL, 0, 0, 0, buffsize, wsh->fd, NULL);
      /* redfine colors and redraw */
      int ctrl_flag = 0;
      pxset_color_map(ws_id);
      pset_colr_rep(ws_id, 0, &(config[ws_id].background_color));
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

/*******************************************************************************
 * pset_ws_vp
 *
 * DESCR:	Set workstation viewport
 * RETURNS:	N/A
 */

void pset_ws_vp(
   Pint ws_id,
   Plimit *viewport
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Plimit3 vp;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_ws_vp);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if (!PHG_IN_RANGE(0.0, dt->dev_coords[0], viewport->x_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[0], viewport->x_max) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[1], viewport->y_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[1], viewport->y_max)) {
         ERR_REPORT(PHG_ERH, ERR157);
      }
      else if (!(viewport->x_min < viewport->x_max) ||
               !(viewport->y_min < viewport->y_max)) {
         ERR_REPORT(PHG_ERH, ERR152);
      }
      else {
         wsh = PHG_WSID(ws_id);
         vp.x_min = viewport->x_min;
         vp.x_max = viewport->x_max;
         vp.y_min = viewport->y_min;
         vp.y_max = viewport->y_max;
         (*wsh->set_ws_vp)(wsh, 1, &vp);
      }
   }
}

/*******************************************************************************
 * pset_ws_vp3
 *
 * DESCR:	Set workstation viewport
 * RETURNS:	N/A
 */

void pset_ws_vp3(
   Pint ws_id,
   Plimit3 *viewport
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_ws_vp3);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if (!PHG_IN_RANGE(0.0, dt->dev_coords[0], viewport->x_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[0], viewport->x_max) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[1], viewport->y_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[1], viewport->y_max) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[2], viewport->z_min) ||
               !PHG_IN_RANGE(0.0, dt->dev_coords[2], viewport->z_max)) {
         ERR_REPORT(PHG_ERH, ERR157);
      }
      else if (!(viewport->x_min < viewport->x_max) ||
               !(viewport->y_min < viewport->y_max) ||
               !(viewport->z_min <= viewport->z_max)) {
         ERR_REPORT(PHG_ERH, ERR152);
      }
      else {
         wsh = PHG_WSID(ws_id);
         (*wsh->set_ws_vp)(wsh, 0, viewport);
      }
   }
}

/*******************************************************************************
 * pset_ws_win
 *
 * DESCR:	Set workstation window
 * RETURNS:	N/A
 */

void pset_ws_win(
   Pint ws_id,
   Plimit *window
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Plimit3 win;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_ws_win);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if (!PHG_IN_RANGE(PDT_NPC_XMIN, PDT_NPC_XMAX, window->x_min) ||
               !PHG_IN_RANGE(PDT_NPC_XMIN, PDT_NPC_XMAX, window->x_max) ||
               !PHG_IN_RANGE(PDT_NPC_YMIN, PDT_NPC_YMAX, window->y_min) ||
               !PHG_IN_RANGE(PDT_NPC_YMIN, PDT_NPC_YMAX, window->y_max)) {
         ERR_REPORT(PHG_ERH, ERR156);
      }
      else if (!(window->x_min < window->x_max) ||
               !(window->y_min < window->y_max)) {
         ERR_REPORT(PHG_ERH, ERR151);
      }
      else {
         wsh = PHG_WSID(ws_id);
         win.x_min = window->x_min;
         win.x_max = window->x_max;
         win.y_min = window->y_min;
         win.y_max = window->y_max;
         (*wsh->set_ws_window)(wsh, 1, &win);
      }
   }
}

/*******************************************************************************
 * pset_ws_win3
 *
 * DESCR:	Set workstation window
 * RETURNS:	N/A
 */

void pset_ws_win3(
   Pint ws_id,
   Plimit3 *window
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_ws_win3);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if (!PHG_IN_RANGE(PDT_NPC_XMIN, PDT_NPC_XMAX, window->x_min) ||
               !PHG_IN_RANGE(PDT_NPC_XMIN, PDT_NPC_XMAX, window->x_max) ||
               !PHG_IN_RANGE(PDT_NPC_YMIN, PDT_NPC_YMAX, window->y_min) ||
               !PHG_IN_RANGE(PDT_NPC_YMIN, PDT_NPC_YMAX, window->y_max) ||
               !PHG_IN_RANGE(PDT_NPC_ZMIN, PDT_NPC_ZMAX, window->z_min) ||
               !PHG_IN_RANGE(PDT_NPC_ZMIN, PDT_NPC_ZMAX, window->z_max)) {
         ERR_REPORT(PHG_ERH, ERR156);
      }
      else if (!(window->x_min < window->x_max) ||
               !(window->y_min < window->y_max) ||
               !(window->z_min <= window->z_max)) {
         ERR_REPORT(PHG_ERH, ERR151);
      }
      else {
         wsh = PHG_WSID(ws_id);
         (*wsh->set_ws_window)(wsh, 0, window);
      }
   }
}

/*******************************************************************************
 * pset_invis_filter
 *
 * DESCR:	Set workstation invisibility filter
 * RETURNS:	N/A
 */

void pset_invis_filter(
   Pint ws_id,
   Pfilter *filter
   )
{
   phg_set_ws_filter(ws_id, Pfn_set_invis_filter, PHG_ARGS_FLT_INVIS, filter);
}

/*******************************************************************************
 * pset_hlhsr_mode
 *
 * DESCR:	Set workstation hlhsr mode
 * RETURNS:	N/A
 */

void pset_hlhsr_mode(
   Pint ws_id,
   Pint hlhsr_mode
   )
{
   int i;
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_set_hlhsr_mode);

   if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      ERR_REPORT(PHG_ERH, ERR3);
   }
   else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
      ERR_REPORT(PHG_ERH, ERR54);
   }
   else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!(dt->ws_category == PCAT_OUT ||
            dt->ws_category == PCAT_TGA ||
            dt->ws_category == PCAT_PNG ||
            dt->ws_category == PCAT_PNGA ||
            dt->ws_category == PCAT_EPS ||
            dt->ws_category == PCAT_PDF ||
            dt->ws_category == PCAT_SVG ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
         ERR_REPORT(PHG_ERH, ERR59);
      }
      for (i = 0; i < dt->num_hlhsr_modes; i++) {
         if (hlhsr_mode == dt->hlhsr_modes[i]) {
            wsh = PHG_WSID(ws_id);
            (*wsh->set_hlhsr_mode)(wsh, hlhsr_mode);
            return;
         }
      }
      ERR_REPORT(PHG_ERH, ERR111);
   }
}

/*******************************************************************************
 * pset_line_rep
 *
 * DESCR:	Set workstation line represenation
 * RETURNS:	N/A
 */

void pset_line_rep(
   Pint ws_id,
   Pint line_ind,
   Pline_bundle *line_bundle
   )
{
   Ws *wsh;
   Phg_args_rep_data rep;
   Wst_phigs_dt *dt;

   dt = phg_wst_check_set_rep(Pfn_set_line_rep,
                              ws_id,
                              line_ind,
                              line_bundle->colr_ind);
   if (dt != NULL) {
      wsh = PHG_WSID(ws_id);
      rep.index = line_ind;
      memcpy(&rep.bundl.lnrep, line_bundle, sizeof(Pline_bundle));
      (*wsh->set_rep)(wsh, PHG_ARGS_LNREP, &rep);
   }
}

/*******************************************************************************
 * pset_marker_rep
 *
 * DESCR:	Set workstation marker represenation
 * RETURNS:	N/A
 */

void pset_marker_rep(
   Pint ws_id,
   Pint marker_ind,
   Pmarker_bundle *marker_bundle
   )
{
   Ws *wsh;
   Phg_args_rep_data rep;
   Wst_phigs_dt *dt;

   dt = phg_wst_check_set_rep(Pfn_set_marker_rep,
                              ws_id,
                              marker_ind,
                              marker_bundle->colr_ind);
   if (dt != NULL) {
      wsh = PHG_WSID(ws_id);
      rep.index = marker_ind;
      memcpy(&rep.bundl.mkrep, marker_bundle, sizeof(Pmarker_bundle));
      (*wsh->set_rep)(wsh, PHG_ARGS_MKREP, &rep);
   }
}

/*******************************************************************************
 * pset_text_rep
 *
 * DESCR:	Set workstation text represenation
 * RETURNS:	N/A
 */

void pset_text_rep(
   Pint ws_id,
   Pint text_ind,
   Ptext_bundle *text_bundle
   )
{
   Ws *wsh;
   Phg_args_rep_data rep;
   Wst_phigs_dt *dt;

   dt = phg_wst_check_set_rep(Pfn_set_text_rep,
                              ws_id,
                              text_ind,
                              text_bundle->colr_ind);
   if (dt != NULL) {
      wsh = PHG_WSID(ws_id);
      rep.index = text_ind;
      memcpy(&rep.bundl.txrep, text_bundle, sizeof(Ptext_bundle));
      (*wsh->set_rep)(wsh, PHG_ARGS_TXREP, &rep);
   }
}

/*******************************************************************************
 * pset_edge_rep
 *
 * DESCR:	Set workstation edge represenation
 * RETURNS:	N/A
 */

void pset_edge_rep(
   Pint ws_id,
   Pint edge_ind,
   Pedge_bundle *edge_bundle
   )
{
   Ws *wsh;
   Phg_args_rep_data rep;
   Wst_phigs_dt *dt;

   dt = phg_wst_check_set_rep(Pfn_set_edge_rep,
                              ws_id,
                              edge_ind,
                              edge_bundle->colr_ind);
   if (dt != NULL) {
      wsh = PHG_WSID(ws_id);
      rep.index = edge_ind;
      memcpy(&rep.bundl.edgerep, edge_bundle, sizeof(Pedge_bundle));
      (*wsh->set_rep)(wsh, PHG_ARGS_EDGEREP, &rep);
   }
}

/*******************************************************************************
 * pset_int_rep
 *
 * DESCR:	Set workstation interior represenation
 * RETURNS:	N/A
 */

void pset_int_rep(
   Pint ws_id,
   Pint int_ind,
   Pint_bundle *int_bundle
   )
{
   Ws *wsh;
   Phg_args_rep_data rep;
   Wst_phigs_dt *dt;

   dt = phg_wst_check_set_rep(Pfn_set_int_rep,
                              ws_id,
                              int_ind,
                              int_bundle->colr_ind);
   if (dt != NULL) {
      wsh = PHG_WSID(ws_id);
      rep.index = int_ind;
      memcpy(&rep.bundl.interrep, int_bundle, sizeof(Pint_bundle));
      (*wsh->set_rep)(wsh, PHG_ARGS_INTERREP, &rep);
   }
}

/*******************************************************************************
 * ppost_struct
 *
 * DESCR:	Post structure to workstation
 * RETURNS:	N/A
 */

void ppost_struct(
   Pint ws_id,
   Pint struct_id,
   Pfloat priority
   )
{
   int status;
   Ws_handle wsh;
   Css_handle cssh;
   Struct_handle structp;

   if (phg_ws_open(ws_id, Pfn_post_struct) != NULL) {
      wsh = PHG_WSID(ws_id);
      cssh = wsh->out_ws.model.b.cssh;
      structp = phg_css_post(cssh, struct_id, wsh, &status);
      if (structp != NULL) {
         (*wsh->post)(wsh, structp, priority, !status);
      }
   }
}

/*******************************************************************************
 * punpost_struct
 *
 * DESCR:       Remove posted structure from workstation
 * RETURNS:     N/A
 */

void punpost_struct(
   Pint ws_id,
   Pint struct_id
   )
{
   Ws_handle wsh;
   Struct_handle structp;

   if (phg_entry_check(PHG_ERH, ERR3, Pfn_unpost_struct)) {
      if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
         ERR_REPORT(PHG_ERH, ERR3);
      }
      else if (!phg_psl_inq_ws_open(PHG_PSL, ws_id)) {
         ERR_REPORT(PHG_ERH, ERR54);
      }
      else {
         wsh = PHG_WSID(ws_id);
         structp = phg_css_unpost(wsh->out_ws.model.b.cssh, struct_id, wsh);
         if (structp != NULL) {
            (*wsh->unpost)(wsh, structp);
         }
      }
   }
}

/*******************************************************************************
 * punpost_all_structs
 *
 * DESCR:       Remove all posted structures from workstation
 * RETURNS:     N/A
 */

void punpost_all_structs(
   Pint ws_id
   )
{
   Ws_handle wsh;

   if (phg_entry_check(PHG_ERH, ERR3, Pfn_unpost_all_structs)) {
      if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
         ERR_REPORT(PHG_ERH, ERR3);
      }
      else if (!phg_psl_inq_ws_open(PHG_PSL, ws_id)) {
         ERR_REPORT(PHG_ERH, ERR54);
      }
      else {
         wsh = PHG_WSID(ws_id);
         (*wsh->unpost_all)(wsh);
      }
   }
}

/*******************************************************************************
 * predraw_all_structs
 *
 * DESCR:	Redraw all structures on workstation
 * RETURNS:	N/A
 */

void predraw_all_structs(
   Pint ws_id,
   Pctrl_flag ctrl_flag
   )
{
   Psl_ws_info *wsinfo;
   Ws_handle wsh;

   wsinfo = phg_ws_open(ws_id, Pfn_redraw_all_structs);
   if (wsinfo != NULL) {
      switch (wsinfo->wstype->desc_tbl.phigs_dt.ws_category) {
         case PCAT_OUTIN:
         case PCAT_OUT:
         case PCAT_TGA:
         case PCAT_PNG:
         case PCAT_PNGA:
         case PCAT_EPS:
         case PCAT_PDF:
         case PCAT_SVG:
         case PCAT_MO:
            wsh = PHG_WSID(ws_id);
            (*wsh->redraw_all)(wsh, ctrl_flag);
            break;

         default:
            ERR_REPORT(PHG_ERH, ERR59);
            break;
      }
   }
}

/*******************************************************************************
 * pupd_ws
 *
 * DESCR:	Set workstation update state
 * RETURNS:	N/A
 */

void pupd_ws(
   Pint ws_id,
   Pregen_flag regen_flag
   )
{
   Psl_ws_info *wsinfo;
   Ws_handle wsh;

   wsinfo = phg_ws_open(ws_id, Pfn_upd_ws);
   if (wsinfo != NULL) {
      switch (wsinfo->wstype->desc_tbl.phigs_dt.ws_category) {
         case PCAT_OUTIN:
         case PCAT_OUT:
         case PCAT_TGA:
         case PCAT_PNG:
         case PCAT_PNGA:
         case PCAT_EPS:
         case PCAT_PDF:
         case PCAT_SVG:
         case PCAT_MO:
            wsh = PHG_WSID(ws_id);
            (*wsh->update)(wsh, regen_flag);
            break;

         default:
            ERR_REPORT(PHG_ERH, ERR59);
            break;
      }
   }
}

/*******************************************************************************
 * pset_disp_upd_st
 *
 * DESCR:	Set workstation update state
 * RETURNS:	N/A
 */

void pset_disp_upd_st(
   Pint ws_id,
   Pdefer_mode def_mode,
   Pmod_mode mod_mode
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;

   wsinfo = phg_ws_open(ws_id, Pfn_set_disp_upd_st);
   if (wsinfo != NULL) {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      switch(dt->ws_category) {
         case PCAT_OUTIN:
         case PCAT_OUT:
         case PCAT_TGA:
         case PCAT_PNG:
         case PCAT_PNGA:
         case PCAT_EPS:
         case PCAT_PDF:
         case PCAT_SVG:
         case PCAT_MO:
            wsh = PHG_WSID(ws_id);
            (*wsh->set_disp_update_state)(wsh, def_mode, mod_mode);
            break;
         default:
            ERR_REPORT(PHG_ERH, ERR59);
            break;
      }
   }
}

/*******************************************************************************
 * pset_view_tran_in_pri
 *
 * DESCR:	Set view input priority
 * RETURNS:	N/A
 */

void pset_view_tran_in_pri(
   Pint ws_id,
   Pint view_ind,
   Pint ref_view_ind,
   Prel_pri rel_pri
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;

   wsinfo = phg_ws_open(ws_id, Pfn_set_view_tran_in_pri);
   if (wsinfo != NULL) {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (dt->ws_category == PCAT_MI) {
         ERR_REPORT(PHG_ERH, ERR57);
      }
      else if ((view_ind < 0) || (ref_view_ind < 0)) {
         ERR_REPORT(PHG_ERH, ERR114);
      }
      /* TODO: Check maximum view index */
      else if (ref_view_ind != view_ind) {
         wsh = PHG_WSID(ws_id);
         (*wsh->set_view_input_priority)(wsh, view_ind, ref_view_ind, rel_pri);
      }
   }
}

/*******************************************************************************
 * pset_light_src_rep
 *
 * DESCR:	Set light source for workstation
 * RETURNS:	N/A
 */

void pset_light_src_rep(
   Pint ws_id,
   Pint light_src_ind,
   Plight_src_bundle *light_src_rep
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Phg_args_rep_data rep;
   Ws_handle wsh;

   wsinfo = phg_ws_open(ws_id, Pfn_set_light_src_rep);
   if (wsinfo != NULL) {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!(dt->ws_category == PCAT_OUT ||
            dt->ws_category == PCAT_TGA ||
            dt->ws_category == PCAT_PNG ||
            dt->ws_category == PCAT_PNGA ||
            dt->ws_category == PCAT_EPS ||
            dt->ws_category == PCAT_PDF ||
            dt->ws_category == PCAT_SVG ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
         ERR_REPORT(PHG_ERH, ERR59);
      }
      else if (light_src_ind < 1) {
         ERR_REPORT(PHG_ERH, ERR129);
      }
      else if ((light_src_rep->type == PLIGHT_SPOT) &&
               ((light_src_rep->rec.spot.angle < 0) ||
                (light_src_rep->rec.spot.angle > M_PI))) {
        ERR_REPORT(PHG_ERH, ERR132);
      }
      else {
         wsh = PHG_WSID(ws_id);
         rep.index = light_src_ind;
         memcpy(&rep.bundl.lightsrcrep,
                light_src_rep,
                sizeof(Plight_src_bundle));
         (*wsh->set_rep)(wsh, PHG_ARGS_LIGHTSRCREP, &rep);
      }
   }
}

/*******************************************************************************
 * pset_colr_rep
 *
 * DESCR:       Set workstation colour representation
 * RETURNS:     N/A
 */

void pset_colr_rep(
   Pint ws_id,
   Pint ind,
   Pcolr_rep *rep
   )
{
   Ws *wsh;
   Phg_args_rep_data corep;
   Wst_phigs_dt *dt;

   dt = phg_wst_check_set_rep(Pfn_set_colr_rep, ws_id, 1, ind);
   if (dt != NULL) {
      wsh = PHG_WSID(ws_id);
      corep.index = ind;
      memcpy(&corep.bundl.corep, rep, sizeof(Pcolr_rep));
      (*wsh->set_rep)(wsh, PHG_ARGS_COREP, &corep);
   }
}

/*******************************************************************************
 * pinq_ws_st
 *
 * DESCR:       Get workstation state
 * RETURNS:     N/A
 */

void pinq_ws_st(
   Pws_st *ws_state
   )
{
   if (phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *ws_state = PSL_WS_STATE(PHG_PSL);
   }
   else {
      *ws_state = PWS_ST_WSCL;
   }
}

/*******************************************************************************
 * pinq_ws_conn_type
 *
 * DESCR:       Get workstation connection type
 * RETURNS:     N/A
 */

void pinq_ws_conn_type(
   Pint ws_id,
   Pstore store,
   Pint *err_ind,
   void **conn_id,
   Pint *ws_type
   )
{
   Psl_ws_info *ws_info;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      ws_info = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (ws_info == NULL) {
         *err_ind = ERR54;
      }
      else {
         *err_ind = 0;
         *((char **) conn_id) = ws_info->connid;
         *ws_type = ws_info->wstype->ws_type;
      }
   }
}

/*******************************************************************************
 * pinq_open_wss
 *
 * DESCR:       Get list of open workstations
 * RETURNS:     N/A
 */

void pinq_open_wss(
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *open_ws_ids,
   Pint *num_elems_impl_list
   )
{
   Pint ws_ids[MAX_NO_OPEN_WS];
   Pint n;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR2;
   }
   else {
      *err_ind = 0;
      n = phg_psl_inq_wsids(PHG_PSL, ws_ids);
      open_ws_ids->num_ints = 0;
      *num_elems_impl_list = n;
      if (n > 0) {
         if (start_ind < 0 || start_ind >= n) {
            *err_ind = ERR2201;
         }
         else if (num_elems_appl_list > 0) {
            open_ws_ids->num_ints = PHG_MIN(num_elems_appl_list, n - start_ind);
            memcpy(open_ws_ids->ints, &ws_ids[start_ind],
                   open_ws_ids->num_ints * sizeof(Pint));
         }
         else if (num_elems_appl_list < 0) {
            *err_ind = ERRN153;
         }
      }
   }
}

/*******************************************************************************
 * pinq_table_indices
 *
 * DESCR:       Get table indices from workstation helper function
 * RETURNS:     N/A
 */

static void pinq_table_indices(
   Phg_args_rep_type type,
   Pint ws_id,
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *def_line_ind,
   Pint *num_elems_impl_list
   )
{
   Ws_handle wsh;
   Phg_ret ret;

   wsh = PHG_WSID(ws_id);
   if (type == PHG_ARGS_VIEWREP) {
      (*wsh->inq_view_indices)(wsh, &ret);
   }
   else {
      (*wsh->inq_bundle_indices)(wsh, type, &ret);
   }

   if (ret.err) {
      *err_ind = ret.err;
   }
   else {
      *err_ind = 0;
      *num_elems_impl_list = ret.data.int_list.num_ints;
      if (ret.data.int_list.num_ints > 0) {
         if (start_ind < 0 || start_ind >= ret.data.int_list.num_ints) {
            *err_ind = ERR2201;
         }
         else if (num_elems_appl_list > 0) {
            def_line_ind->num_ints =
               PHG_MIN(num_elems_appl_list,
                       ret.data.int_list.num_ints - start_ind);
            memcpy (def_line_ind->ints,
                    &ret.data.int_list.ints[start_ind],
                    def_line_ind->num_ints * sizeof(Pint));
         }
         else if (num_elems_appl_list < 0) {
            *err_ind = ERRN153;
         }
      }
   }
}

/*******************************************************************************
 * pinq_list_view_inds
 *
 * DESCR:       Get workstation list of view indices
 * RETURNS:     N/A
 */

void pinq_list_view_inds(
   Pint ws_id,
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *view_inds,
   Pint *num_elems_impl_list
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (dt->ws_category == PCAT_MI) {
            *err_ind = ERR57;
         }
         else {
            pinq_table_indices(PHG_ARGS_VIEWREP, ws_id, num_elems_appl_list,
                               start_ind, err_ind,
                               view_inds, num_elems_impl_list);
         }
      }
   }
}

/*******************************************************************************
 * pinq_list_line_inds
 *
 * DESCR:       Get workstation list of line indices
 * RETURNS:     N/A
 */

void pinq_list_line_inds(
   Pint ws_id,
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *def_line_ind,
   Pint *num_elems_impl_list
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else {
            pinq_table_indices(PHG_ARGS_LNREP, ws_id, num_elems_appl_list,
                               start_ind, err_ind,
                               def_line_ind, num_elems_impl_list);
         }
      }
   }
}

/*******************************************************************************
 * pinq_list_marker_inds
 *
 * DESCR:       Get workstation list of marker indices
 * RETURNS:     N/A
 */

void pinq_list_marker_inds(
   Pint ws_id,
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *def_marker_ind,
   Pint *num_elems_impl_list
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else {
            pinq_table_indices(PHG_ARGS_MKREP, ws_id, num_elems_appl_list,
                               start_ind, err_ind,
                               def_marker_ind, num_elems_impl_list);
         }
      }
   }
}

/*******************************************************************************
 * pinq_list_text_inds
 *
 * DESCR:       Get workstation list of text indices
 * RETURNS:     N/A
 */

void pinq_list_text_inds(
   Pint ws_id,
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *def_text_ind,
   Pint *num_elems_impl_list
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else {
            pinq_table_indices(PHG_ARGS_TXREP, ws_id, num_elems_appl_list,
                               start_ind, err_ind,
                               def_text_ind, num_elems_impl_list);
         }
      }
   }
}

/*******************************************************************************
 * pinq_list_int_inds
 *
 * DESCR:       Get workstation list of interior indices
 * RETURNS:     N/A
 */

void pinq_list_int_inds(
   Pint ws_id,
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *def_int_ind,
   Pint *num_elems_impl_list
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else {
            pinq_table_indices(PHG_ARGS_INTERREP, ws_id, num_elems_appl_list,
                               start_ind, err_ind,
                               def_int_ind, num_elems_impl_list);
         }
      }
   }
}

/*******************************************************************************
 * pinq_list_edge_inds
 *
 * DESCR:       Get workstation list of edge indices
 * RETURNS:     N/A
 */

void pinq_list_edge_inds(
   Pint ws_id,
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *def_edge_ind,
   Pint *num_elems_impl_list
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else {
            pinq_table_indices(PHG_ARGS_EDGEREP, ws_id, num_elems_appl_list,
                               start_ind, err_ind,
                               def_edge_ind, num_elems_impl_list);
         }
      }
   }
}

/*******************************************************************************
 * pinq_list_colr_inds
 *
 * DESCR:       Get workstation list of colour indices
 * RETURNS:     N/A
 */

void pinq_list_colr_inds(
   Pint ws_id,
   Pint num_elems_appl_list,
   Pint start_ind,
   Pint *err_ind,
   Pint_list *colr_ind,
   Pint *num_elems_impl_list
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else {
            pinq_table_indices(PHG_ARGS_COREP, ws_id, num_elems_appl_list,
                               start_ind, err_ind,
                               colr_ind, num_elems_impl_list);
         }
      }
   }
}

/*******************************************************************************
 * pinq_line_rep
 *
 * DESCR:       Get workstation line representation
 * RETURNS:     N/A
 */

void pinq_line_rep(
   Pint ws_id,
   Pint index,
   Pinq_type type,
   Pint *err_ind,
   Pline_bundle *line_rep
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Phg_ret ret;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else if (index < 1) {
            *err_ind = ERR100;
         }
         else {
            wsh = PHG_WSID(ws_id);
            (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_EXTLNREP,
                                       &ret);
            if (ret.err) {
               *err_ind = ret.err;
            }
            else if (ret.data.rep.extlnrep.colr.type != PINDIRECT) {
               *err_ind = ERR134;
            }
            else {
               line_rep->type = ret.data.rep.extlnrep.type;
               line_rep->width = ret.data.rep.extlnrep.width;
               line_rep->colr_ind = ret.data.rep.extlnrep.colr.val.ind;
               *err_ind = 0;
            }
         }
      }
   }
}

/*******************************************************************************
 * pinq_marker_rep
 *
 * DESCR:       Get workstation marker representation
 * RETURNS:     N/A
 */

void pinq_marker_rep(
   Pint ws_id,
   Pint index,
   Pinq_type type,
   Pint *err_ind,
   Pmarker_bundle *marker_rep
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Phg_ret ret;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else if (index < 1) {
            *err_ind = ERR100;
         }
         else {
            wsh = PHG_WSID(ws_id);
            (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_EXTMKREP,
                                       &ret);
            if (ret.err) {
               *err_ind = ret.err;
            }
            else if (ret.data.rep.extmkrep.colr.type != PINDIRECT) {
               *err_ind = ERR134;
            }
            else {
               marker_rep->type = ret.data.rep.extmkrep.type;
               marker_rep->size = ret.data.rep.extmkrep.size;
               marker_rep->colr_ind = ret.data.rep.extmkrep.colr.val.ind;
               *err_ind = 0;
            }
         }
      }
   }
}

/*******************************************************************************
 * pinq_text_rep
 *
 * DESCR:       Get workstation text representation
 * RETURNS:     N/A
 */

void pinq_text_rep(
   Pint ws_id,
   Pint index,
   Pinq_type type,
   Pint *err_ind,
   Ptext_bundle *text_rep
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Phg_ret ret;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else if (index < 1) {
            *err_ind = ERR100;
         }
         else {
            wsh = PHG_WSID(ws_id);
            (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_EXTTXREP,
                                       &ret);
            if (ret.err) {
               *err_ind = ret.err;
            }
            else if (ret.data.rep.exttxrep.colr.type != PINDIRECT) {
               *err_ind = ERR134;
            }
            else {
               text_rep->font = ret.data.rep.exttxrep.font;
               text_rep->prec = ret.data.rep.exttxrep.prec;
               text_rep->char_expan = ret.data.rep.exttxrep.char_expan;
               text_rep->char_space = ret.data.rep.exttxrep.char_space;
               text_rep->colr_ind = ret.data.rep.exttxrep.colr.val.ind;
               *err_ind = 0;
            }
         }
      }
   }
}

/*******************************************************************************
 * pinq_int_rep
 *
 * DESCR:       Get workstation interior representation
 * RETURNS:     N/A
 */

void pinq_int_rep(
   Pint ws_id,
   Pint index,
   Pinq_type type,
   Pint *err_ind,
   Pint_bundle *int_rep
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Phg_ret ret;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else if (index < 1) {
            *err_ind = ERR100;
         }
         else {
            wsh = PHG_WSID(ws_id);
            (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_EXTINTERREP,
                                       &ret);
            if (ret.err) {
               *err_ind = ret.err;
            }
            else if (ret.data.rep.extinterrep.colr.type != PINDIRECT) {
               *err_ind = ERR134;
            }
            else {
               int_rep->style = ret.data.rep.extinterrep.style;
               int_rep->style_ind = ret.data.rep.extinterrep.style_ind;
               int_rep->colr_ind = ret.data.rep.extinterrep.colr.val.ind;
               *err_ind = 0;
            }
         }
      }
   }
}

/*******************************************************************************
 * pinq_edge_rep
 *
 * DESCR:       Get workstation edge representation
 * RETURNS:     N/A
 */

void pinq_edge_rep(
   Pint ws_id,
   Pint index,
   Pinq_type type,
   Pint *err_ind,
   Pedge_bundle *edge_rep
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Phg_ret ret;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else if (index < 1) {
            *err_ind = ERR100;
         }
         else {
            wsh = PHG_WSID(ws_id);
            (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_EXTEDGEREP,
                                       &ret);
            if (ret.err) {
               *err_ind = ret.err;
            }
            else if (ret.data.rep.extedgerep.colr.type != PINDIRECT) {
               *err_ind = ERR134;
            }
            else {
               edge_rep->flag = ret.data.rep.extedgerep.flag;
               edge_rep->type = ret.data.rep.extedgerep.type;
               edge_rep->width = ret.data.rep.extedgerep.width;
               edge_rep->colr_ind = ret.data.rep.extedgerep.colr.val.ind;
               *err_ind = 0;
            }
         }
      }
   }
}

/*******************************************************************************
 * pinq_colr_rep
 *
 * DESCR:       Get workstation colour representation
 * RETURNS:     N/A
 */

void pinq_colr_rep(
   Pint ws_id,
   Pint colr_ind,
   Pinq_type type,
   Pint *err_ind,
   Pcolr_rep *colr_rep
   )
{
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   Phg_ret ret;

   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else if (colr_ind < 0) {
            *err_ind = ERR113;
         }
         else {
            wsh = PHG_WSID(ws_id);
            (*wsh->inq_representation)(wsh, colr_ind, type, PHG_ARGS_COREP,
                                       &ret);
            if (ret.err) {
               *err_ind = ret.err;
            }
            else {
               memcpy (colr_rep, &ret.data.rep.corep, sizeof(Pcolr_rep));
               *err_ind = 0;
            }
         }
      }
   }
}

/*******************************************************************************
 * inq_filter
 *
 * DESCR:       Get workstation filter
 * RETURNS:     N/A
 */

static void inq_filter(
   Phg_args_flt_type type,
   Pint ws_id,
   struct _Pstore *store,
   Pint *err_ind,
   Pfilter **filter
   )
{
   Phg_ret ret;
   Phg_ret_filter *pf = &ret.data.filter;
   Psl_ws_info *wsinfo;
   Wst_phigs_dt *dt;
   Ws_handle wsh;
   int size;
   if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
      *err_ind = ERR3;
   }
   else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
      *err_ind = ERR3;
   }
   else {
      wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
      if (wsinfo == NULL) {
         *err_ind = ERR54;
      }
      else {
         dt = &wsinfo->wstype->desc_tbl.phigs_dt;
         if (!(dt->ws_category == PCAT_OUT ||
               dt->ws_category == PCAT_TGA ||
               dt->ws_category == PCAT_PNG ||
               dt->ws_category == PCAT_PNGA ||
               dt->ws_category == PCAT_EPS ||
               dt->ws_category == PCAT_PDF ||
               dt->ws_category == PCAT_SVG ||
               dt->ws_category == PCAT_OUTIN ||
               dt->ws_category == PCAT_MO)) {
            *err_ind = ERR59;
         }
         else {
            wsh = PHG_WSID(ws_id);
            (*wsh->inq_filter)(wsh, type, &ret);
            if (ret.err) {
               *err_ind = ret.err;
            }
            else {
               *err_ind = 0;
               size = (pf->incl.num_ints + pf->excl.num_ints) * sizeof(Pint);
               if (phg_cb_resize_store(store, size, err_ind)) {
                  *filter = &store->data.filter;
                  (*filter)->incl_set.num_ints = pf->incl.num_ints;
                  (*filter)->excl_set.num_ints = pf->excl.num_ints;
                  (*filter)->incl_set.ints = (Pint *) store->buf;
                  (*filter)->excl_set.ints =
                     &(*filter)->incl_set.ints[(*filter)->incl_set.num_ints];
                  if (pf->incl.num_ints > 0) {
                     memcpy((*filter)->incl_set.ints, pf->incl.ints,
                            (*filter)->incl_set.num_ints * sizeof(Pint));
                  }
                  if (pf->excl.num_ints > 0) {
                     memcpy((*filter)->excl_set.ints, pf->excl.ints,
                            (*filter)->excl_set.num_ints * sizeof(Pint));
                  }
               }
            }
         }
      }
   }
}

/*******************************************************************************
 * pinq_invis_filter
 *
 * DESCR:       Get workstation invisibility filter
 * RETURNS:     N/A
 */

void pinq_invis_filter(
   Pint ws_id,
   Pstore store,
   Pint *err_ind,
   Pfilter **invis_filter
   )
{
   inq_filter(PHG_ARGS_FLT_INVIS, ws_id, ((struct _Pstore *) store),
              err_ind, invis_filter);
}

/***********************************
 * predefine some colors
 *
 **********************************/
void pxset_color_map(Pint ws_id){
  int i, j, k;
  int n = 5;
  int index = 0;
  float delta_n = 1.0/(n-1);
  Pcolr_rep rep;
  for (i=0; i<5; i++){
    for (j=0; j<5; j++){
      for (k=0; k<5; k++){
        rep.rgb.red   = i*delta_n;
        rep.rgb.green = j*delta_n;
        rep.rgb.blue  = k*delta_n;
        pset_colr_rep(ws_id, 16+index, &rep);
#ifdef DEBUG
        printf("Defining color index %d as RGB %f %f %f\n", 16+index, rep.rgb.red, rep.rgb.green, rep.rgb.blue);
#endif
        index += 1;
      }
    }
  }
}

/*******************************************************************************
 * pmessage
 *
 * DESCR:	Display a message box
 * RETURNS:	N/A
 */

void pmessage(
	      Pint ws_id,
	      char* message
	      )
{
  Phg_args_message args;
  Ws_handle wsh;
  wsh = PHG_WSID(ws_id);
  args.wsid = ws_id;
  args.msg = message;
  args.msg_length = strlen(message);
  (*wsh->message)(wsh, &args);
}
