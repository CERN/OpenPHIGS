/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
*             (C) 2022-2023 CERN
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
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "phigs.h"
#include "phg.h"
#include "css.h"
#include "ws.h"
#include "private/phgP.h"
#include "private/cbP.h"

/*******************************************************************************
 * pinq_light_src_rep
 *
 * DESCR:       inquire light source respresentation
 * RETURNS:     N/A
 */

void pinq_light_src_rep( ws_id, index, type, err_ind, rep)
     Pint  ws_id;
     Pint  index;
     Pinq_type  type;
     Pint *err_ind;
     Plight_src_bundle *rep;
{
  Phg_ret         ret;
  Psl_ws_info     *wsinfo;
  Wst_phigs_dt    *dt;
  Ws_handle wsh;

  *err_ind = 0;
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
    } else {
      dt = &((Wst*)wsinfo->wstype)->desc_tbl.phigs_dt;
      if ( !(dt->ws_category == PCAT_OUT ||
             dt->ws_category == PCAT_TGA ||
             dt->ws_category == PCAT_PNG ||
             dt->ws_category == PCAT_PNGA ||
             dt->ws_category == PCAT_EPS ||
             dt->ws_category == PCAT_PDF ||
             dt->ws_category == PCAT_SVG ||
             dt->ws_category == PCAT_OUTIN ||
             dt->ws_category == PCAT_MO) ) {
        *err_ind = ERR59;
      } else if ( index < 1) {
        *err_ind = ERR129;
      } else {
        wsh = PHG_WSID(ws_id);
        (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_LIGHTSRCREP, &ret);
        if ( ret.err) {
          *err_ind = ret.err;
        } else {
          *rep = ret.data.rep.lightsrcrep;
        }
      }
    }
  }
}
