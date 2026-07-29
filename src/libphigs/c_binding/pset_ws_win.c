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
 * \file pset_ws_win.c
 * \brief Set workstation window
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

