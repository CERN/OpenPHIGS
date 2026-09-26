/******************************************************************************
 *   Do NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
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

#include <gl2ps.h>
#include "phigs.h"
#include "phg.h"
#include "private/phgP.h"
#include "private/wsglP.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"
#include "private/cb_internal.h"

/** \internal recording flag for printing */
extern int record_geom;

/**
 * \brief       Open workstation
 *
 * \file popwk.c
 *
 * \param       wkid   workstation ID
 * \param       conid  connection ID
 * \param       wkid   wktype workstation type
 *
 * \verbatim
  0  PWST_OUTPUT_TRUE                 Output only on GL display
  1  PWST_OUTIN_TRUE                  Input/Output on GL display
  2  PWST_OUTPUT_TRUE_DB              Output only on GL, buffered
  3  PWST_OUTIN_TRUE_DB               Input/Output on GL display, buffered
  4  PWST_HCOPY_TRUE_TGA              Hardcopy to file as TGA
  5  PWST_HCOPY_TRUE_RGB_PNG          Hardcopy to file as PNG RGB only
  6  PWST_HCOPY_TRUE_RGBA_PNG         Hardcopy to file as PNG with Alpha channel
  7  PWST_HCOPY_TRUE_EPS              Hardcopy to file as Encapsulated PostScript, no shaders
  8  PWST_HCOPY_TRUE_PDF              Hardcopy to file as PDF, no shaders
  9  PWST_HCOPY_TRUE_SVG              Hardcopy to file as SVG, no shaders
  10 PWST_HCOPY_TRUE_OBJ              Export geometry as OBJ
  11 PWST_HCOPY_TRUE_GLTF             Export geometry as GLTF
 * \endverbatim
 *
 * \note The implementation for geometry export as OBJ is relatively basic and does not include any material or color schemas. The scale factor for TGA and PNG output can be set with the function PXSHCSF.
 *
 * The second argument of popwk expects a non-zero integer which will be passed on as logical unit number.
 *   - If a file name has been defined in the configuration file as  \%wf \<filename\>, the targa file will be created with this name in the current folder.
 *   - If no file name has been defined in the configuration file as \%wf \<filename\> fort.\<LUN\> will be used.
 *   - After opening the work station, the output file name can be set with a call to CALL PSFNAME(WKD, \<filename\>) where filename can be the full path to the output
 *
 * \pre The workstation must not be open yet.
 * \bug None know at the moment.
 *
 * \sa pclwk pxshcsf pxndef
 */
FTN_SUBROUTINE(popwk)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(conid),
                      FTN_INTEGER(wtype)
                      )
{
  Wst *wst;
  Ws_handle wsh;
  Phg_args_open_ws args;
  Phg_ret ret;

  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint lun = FTN_INTEGER_GET(conid);
  Pint ws_type = FTN_INTEGER_GET(wtype);

  Phg_args_conn_info conn_id;
  Pcolr_rep rep;
  char filename[512];

  /* init filename to zero */
  bzero(filename, 512);
  conn_id.lun = lun;
  conn_id.background = 0;

  /* read default configuration file if not read yet */
  if (! config_read){
    config_read = 1;
    read_config("phigs.def");
  };
  /* save the current shader settings */
  wsgl_use_shaders_settings = wsgl_use_shaders;
  if (phg_entry_check(PHG_ERH, ERR2, Pfn_open_ws)) {

#ifdef DEBUG
    printf("DEBUG: POPWK open %d\n", *wkid);
#endif

    ERR_SET_CUR_FUNC(PHG_ERH, Pfn_open_ws);

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
        printf("fb_ws: WSID=%d type=%d scale factor %f\n", ws_id, ws_type, args.hcsf);
#endif
        if (lun == 0) {
          args.conn_info.background = 0;
          args.conn_type = PHG_ARGS_CONN_OPEN;
        }
        else {
          record_geom = FALSE;
          if (
              ws_type == PWST_HCOPY_TRUE_TGA ||
              ws_type == PWST_HCOPY_TRUE_RGB_PNG ||
              ws_type == PWST_HCOPY_TRUE_RGBA_PNG ||
              ws_type == PWST_HCOPY_TRUE_EPS ||
              ws_type == PWST_HCOPY_TRUE_PDF ||
              ws_type == PWST_HCOPY_TRUE_SVG ||
              ws_type == PWST_HCOPY_TRUE_OBJ ||
              ws_type == PWST_HCOPY_TRUE_GLTF
              ) {
            args.conn_type = PHG_ARGS_CONN_HCOPY;
            args.width = config[ws_id].display_width*config[ws_id].hcsf;
            args.height = config[ws_id].display_height*config[ws_id].hcsf;
            memcpy(&args.conn_info, &conn_id, sizeof(Phg_args_conn_info));
          }
          else {
            args.conn_type = PHG_ARGS_CONN_DRAWABLE;
            memcpy(&args.conn_info, &conn_id, sizeof(Phg_args_conn_info));
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
        case  PWST_HCOPY_TRUE_GLTF:
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
        args.oir_mode = config[ws_id].oir;
        args.layersPerPixel = config[ws_id].layersPerPixel;

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
        /* init output file name */
        wsh = PHG_WSID(ws_id);
        /* set background as specified in configuration file */
        if (wsh->current_colour_model == PMODEL_RGBA){
          pset_colr_rep(ws_id, 0, &(config[ws_id].background_color_rgba));
        } else {
          pset_colr_rep(ws_id, 0, &(config[ws_id].background_color_rgb));
        }
        if (strlen(config[ws_id].filename) == 0){
          sprintf(filename, "fort.%d", lun);
          strncpy(wsh->filename, filename, strlen(filename));
          (wsh->filename)[strlen(filename)] = '\0';
        } else {
          strncpy(wsh->filename, config[ws_id].filename, strlen(config[ws_id].filename));
          (wsh->filename)[strlen(config[ws_id].filename)] = '\0';
        }
        wsgl_clear(wsh);
      }
    }
    ERR_FLUSH(PHG_ERH);
  }
}
