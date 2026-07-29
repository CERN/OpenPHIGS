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
 * \file pupd_ws.c
 * \brief Set workstation update state
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
    case PCAT_OBJ:
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

