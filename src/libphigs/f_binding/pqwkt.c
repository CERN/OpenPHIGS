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
#include "private/cb_internal.h"
#include "private/wsglP.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"

extern short int wsgl_use_shaders_settings;
extern int record_geom;

/**
 * \file pqwkt.c
 * \brief Inquire workstation transformation 3
 *
 * \return error index, update state, requested window in NPC, current window in NPC, requested window in DC, current window in DC
 */

FTN_SUBROUTINE(pqwkt)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(err_ind),
                      FTN_INTEGER(tus),
                      Pfloat*rwindo,
                      Pfloat*cwindo,
                      Pfloat*rviewp,
                      Pfloat*cviewp
                      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);

  Pinq_type type;
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Phg_ret ret;

#ifdef DEBUG
  printf("DEBUG: pqwkt called\n");
#endif

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
            dt->ws_category == PCAT_EPS  ||
            dt->ws_category == PCAT_PDF  ||
            dt->ws_category == PCAT_SVG  ||
            dt->ws_category == PCAT_OBJ ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else {
        wsh = PHG_WSID(ws_id);
        (*wsh->inq_disp_update_state)(wsh, &ret);
        if (ret.err) {
          *err_ind = ret.err;
        } else {
          Wsb_output_ws   *owsb = &wsh->out_ws.model.b;
          if (owsb->ws_window_pending || owsb->ws_viewport_pending) {*tus = 1;} else {*tus = 0;};
          rwindo[0] = owsb->req_ws_window.x_min;
          rwindo[1] = owsb->req_ws_window.x_max;
          rwindo[2] = owsb->req_ws_window.y_min;
          rwindo[3] = owsb->req_ws_window.y_max;

          cwindo[0] = owsb->ws_window.x_min;
          cwindo[1] = owsb->ws_window.x_max;
          cwindo[2] = owsb->ws_window.y_min;
          cwindo[3] = owsb->ws_window.y_max;

          rviewp[0] = owsb->req_ws_viewport.x_min;
          rviewp[1] = owsb->req_ws_viewport.x_max;
          rviewp[2] = owsb->req_ws_viewport.y_min;
          rviewp[3] = owsb->req_ws_viewport.y_max;

          cviewp[0] = owsb->ws_viewport.x_min;
          cviewp[1] = owsb->ws_viewport.x_max;
          cviewp[2] = owsb->ws_viewport.y_min;
          cviewp[3] = owsb->ws_viewport.y_max;
          *err_ind = 0;
        }
      }
    }
  }
}

