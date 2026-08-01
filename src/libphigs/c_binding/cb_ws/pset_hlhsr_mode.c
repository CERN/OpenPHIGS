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
 * \file pset_hlhsr_mode.c
 * \brief Set workstation hlhsr mode
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
          dt->ws_category == PCAT_OBJ ||
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

