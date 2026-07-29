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
 * \file pqpost.c
 * \brief Inquire posted structures
 *
 * \return error index, HLHRS mode update status, current HLHRS mode, requested HLHRS mode
 */

FTN_SUBROUTINE(pqpost)(
                       FTN_INTEGER(wkid),
                       FTN_INTEGER(n),
                       FTN_INTEGER(err_ind),
                       FTN_INTEGER(number),
                       FTN_INTEGER(strid),
                       FTN_REAL(priort)
                       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint num = FTN_INTEGER_GET(n);
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Ws_post_str * current;
  Wsb_output_ws *owsb;
  Ws_posted_structs posted;
  int nposted, str_id;
  float prio;

#ifdef DEBUG
  printf("DEBUG: PQPOST inquire work station \n");
#endif
  nposted = 0;
  str_id = 0;
  prio = 0.0;
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
        owsb = &wsh->out_ws.model.b;
        Ws_posted_structs posted = owsb->posted;
        current = &posted.highest;
        while (current != NULL) {
          if (current->structh != NULL) {
            nposted += 1;
            prio = current->disp_pri;
            str_id = current->structh->struct_id;
            if (nposted == num) {
              *strid = str_id;
              *priort = prio;
            }
          }
          current = current->lower;
        }
        *err_ind = 0;
        *number = nposted;
#ifdef DEBUG
        printf("PQPOST: returning number %d, strid %d and prio %f\n", nposted, *strid, *priort);
#endif
      }
    }
  }
}

