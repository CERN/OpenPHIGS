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

/**
 * \file pset_disp_upd_st.c
 * \brief Set workstation update state
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
    case PCAT_OBJ:
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

