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
 * \file pset_view_tran_in_pri.c
 * \brief Set view input priority
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

