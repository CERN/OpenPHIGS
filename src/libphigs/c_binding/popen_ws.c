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
#include "private/cb_internal.h"
#include "private/cbP.h"
#include "private/wsglP.h"
#include "private/wsxP.h"
#include "phconf.h"

short int wsgl_use_shaders_settings;

/**
 * \file popen_ws.c
 * \brief Open workstation
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
          record_geom = FALSE;
          if (
              ws_type == PWST_HCOPY_TRUE_TGA ||
              ws_type == PWST_HCOPY_TRUE_RGB_PNG ||
              ws_type == PWST_HCOPY_TRUE_RGBA_PNG ||
              ws_type == PWST_HCOPY_TRUE_EPS ||
              ws_type == PWST_HCOPY_TRUE_PDF ||
              ws_type == PWST_HCOPY_TRUE_SVG ||
              ws_type == PWST_HCOPY_TRUE_OBJ
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
        switch (ws_type){
        case PWST_HCOPY_TRUE_EPS:
        case PWST_HCOPY_TRUE_PDF:
        case PWST_HCOPY_TRUE_SVG:
          /* switch off shaders for gl2ps exports */
          wsgl_use_shaders_settings = wsgl_use_shaders;
          wsgl_use_shaders = 0;
          break;
        case  PWST_HCOPY_TRUE_OBJ:
          record_geom = TRUE;
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
        wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
        dt = &wsinfo->wstype->desc_tbl.phigs_dt;
        /* init the file name */
        wsh = PHG_WSID(ws_id);
        /* set background as specified in configuration file */
        if (wsh->current_colour_model == PMODEL_RGBA){
          pset_colr_rep(ws_id, 0, &(config[ws_id].background_color_rgba));
        } else {
          pset_colr_rep(ws_id, 0, &(config[ws_id].background_color_rgb));
        }
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
          case PCAT_OBJ:
            strcpy(wsh->filename, "output.obj");
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

