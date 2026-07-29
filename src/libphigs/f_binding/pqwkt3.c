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

extern short int wsgl_use_shaders_settings;
extern int record_geom;

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(popwk)(;
extern FTN_SUBROUTINE(ppost)(;
extern FTN_SUBROUTINE(pscm)(;
extern FTN_SUBROUTINE(pscr)(;
extern FTN_SUBROUTINE(pupost)(;
extern FTN_SUBROUTINE(pupast)(;
extern FTN_SUBROUTINE(prst)(;
extern FTN_SUBROUTINE(pswkw)(;
extern FTN_SUBROUTINE(pswkw3)(;
extern FTN_SUBROUTINE(pclwk)(;
extern FTN_SUBROUTINE(psvwr3)(;
extern FTN_SUBROUTINE(pqhrm)(;
extern FTN_SUBROUTINE(pqvwr)(;
extern FTN_SUBROUTINE(pqwkt)(;
extern FTN_SUBROUTINE(pqwkt3)(;
extern FTN_SUBROUTINE(pqpost)(;
extern FTN_SUBROUTINE(pqwkpo)(;
extern FTN_SUBROUTINE(puwk)(;
extern FTN_SUBROUTINE(pspmr)(;
extern FTN_SUBROUTINE(psplr)(;
extern FTN_SUBROUTINE(psedr)(;
extern FTN_SUBROUTINE(psir)(;
extern FTN_SUBROUTINE(pstxr)(;
extern FTN_SUBROUTINE(pswkv3)(;
extern FTN_SUBROUTINE(psdus)(;
extern FTN_SUBROUTINE(pqcr)(;
extern FTN_SUBROUTINE(pqcf)(;
extern FTN_SUBROUTINE(pmsg)(;
extern FTN_SUBROUTINE(pslsr)(;
extern FTN_SUBROUTINE(pxscm)(;
extern FTN_SUBROUTINE(psfname)(;


/**
 * \file pqwkt3.c
 * \brief Inquire workstation transformation 3
 *
 * \return error index, update status, requested window in NPC, current window in NPC, requested viewport in DC, current viewport in DC,
 */

FTN_SUBROUTINE(pqwkt3)(
                       FTN_INTEGER(wkid),
                       FTN_INTEGER(err_ind),
                       FTN_INTEGER(tus),
                       Pfloat* rwindo,
                       Pfloat* cwindo,
                       Pfloat* rviewp,
                       Pfloat* cviewp
                       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);

  Pinq_type type;
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Phg_ret ret;
#ifdef DEBUG
  printf("DEBUG: pqwkt3 called\n");
#endif

  if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
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
            dt->ws_category == PCAT_OBJ ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else {
        wsh = PHG_WSID(ws_id);
        Wsb_output_ws   *owsb = &wsh->out_ws.model.b;
        if (owsb->ws_window_pending || owsb->ws_viewport_pending) {*tus = 1;} else {*tus = 0;};
        rwindo[0] = owsb->req_ws_window.x_min;
        rwindo[1] = owsb->req_ws_window.x_max;
        rwindo[2] = owsb->req_ws_window.y_min;
        rwindo[3] = owsb->req_ws_window.y_max;
        rwindo[4] = owsb->req_ws_window.z_min;
        rwindo[5] = owsb->req_ws_window.z_max;

        cwindo[0] = owsb->ws_window.x_min;
        cwindo[1] = owsb->ws_window.x_max;
        cwindo[2] = owsb->ws_window.y_min;
        cwindo[3] = owsb->ws_window.y_max;
        cwindo[4] = owsb->ws_window.z_min;
        cwindo[5] = owsb->ws_window.z_max;

        rviewp[0] = owsb->req_ws_viewport.x_min;
        rviewp[1] = owsb->req_ws_viewport.x_max;
        rviewp[2] = owsb->req_ws_viewport.y_min;
        rviewp[3] = owsb->req_ws_viewport.y_max;
        rviewp[4] = owsb->req_ws_viewport.z_min;
        rviewp[5] = owsb->req_ws_viewport.z_max;

        cviewp[0] = owsb->ws_viewport.x_min;
        cviewp[1] = owsb->ws_viewport.x_max;
        cviewp[2] = owsb->ws_viewport.y_min;
        cviewp[3] = owsb->ws_viewport.y_max;
        cviewp[4] = owsb->ws_viewport.z_min;
        cviewp[5] = owsb->ws_viewport.z_max;
        *err_ind = 0;
      }
    }
  }
}

