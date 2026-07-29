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
 * \file psvwr3.c
 * \brief Set view representation 3
 */

FTN_SUBROUTINE(psvwr3)(
                       FTN_INTEGER(wkid),
                       FTN_INTEGER(viewi),
                       Pfloat* vwormti,
                       Pfloat* vwmpmti,
                       FTN_REAL_ARRAY(vwcplm),
                       FTN_INTEGER(xclipi),
                       FTN_INTEGER(bclipi),
                       FTN_INTEGER(fclipi)
                       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint vrep = FTN_INTEGER_GET(viewi);
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Phg_args_rep_data rep;
  Pint xcli = FTN_INTEGER_GET(xclipi);
  Pint bcli = FTN_INTEGER_GET(bclipi);
  Pint fcli = FTN_INTEGER_GET(fclipi);
  Pmatrix3 vwormt;
  Pmatrix3 vwmpmt;
  Ws_handle wsh;
  Phg_ret ret;
  int i ,j;
#ifdef DEBUG
  printf("DEBUG: psvwr3 changing view %d\n", vrep);
#endif
  if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    printf("FIXME: Error in psvwr3\n");
  }
  else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
    printf("Error ERR54 in psvwr3\n");
  }
  else {
    dt = &wsinfo->wstype->desc_tbl.phigs_dt;
    if (dt->ws_category == PCAT_MI) {
      printf("Error ERR57 in psvwr3\n");
    }
    wsh = PHG_WSID(ws_id);
    rep.index = vrep;
    for (i=0; i<4; i++){
      for (j=0; j<4; j++){
        rep.bundl.viewrep.ori_matrix[i][j] = (Pfloat)vwormti[i+4*j];
        rep.bundl.viewrep.map_matrix[i][j] = (Pfloat)vwmpmti[i+4*j];
      }
    }
    rep.bundl.viewrep.clip_limit.x_min = FTN_REAL_ARRAY_GET(vwcplm, 0);
    rep.bundl.viewrep.clip_limit.x_max = FTN_REAL_ARRAY_GET(vwcplm, 1);
    rep.bundl.viewrep.clip_limit.y_min = FTN_REAL_ARRAY_GET(vwcplm, 2);
    rep.bundl.viewrep.clip_limit.y_max = FTN_REAL_ARRAY_GET(vwcplm, 3);
    rep.bundl.viewrep.clip_limit.z_min = FTN_REAL_ARRAY_GET(vwcplm, 4);
    rep.bundl.viewrep.clip_limit.z_max = FTN_REAL_ARRAY_GET(vwcplm, 5);
    rep.bundl.viewrep.xy_clip = xcli;
    rep.bundl.viewrep.back_clip = bcli;
    rep.bundl.viewrep.front_clip = fcli;

    (*wsh->set_rep)(wsh, PHG_ARGS_VIEWREP, &rep);
#ifdef DEBUG
    printf("ORI:\n");
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.ori_matrix[0][0], rep.bundl.viewrep.ori_matrix[1][0], rep.bundl.viewrep.ori_matrix[2][0], rep.bundl.viewrep.ori_matrix[3][0]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.ori_matrix[0][1], rep.bundl.viewrep.ori_matrix[1][1], rep.bundl.viewrep.ori_matrix[2][1], rep.bundl.viewrep.ori_matrix[3][1]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.ori_matrix[0][2], rep.bundl.viewrep.ori_matrix[1][2], rep.bundl.viewrep.ori_matrix[2][2], rep.bundl.viewrep.ori_matrix[3][2]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.ori_matrix[0][3], rep.bundl.viewrep.ori_matrix[1][3], rep.bundl.viewrep.ori_matrix[2][3], rep.bundl.viewrep.ori_matrix[3][3]);
    printf("REP:\n");
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.map_matrix[0][0], rep.bundl.viewrep.map_matrix[1][0], rep.bundl.viewrep.map_matrix[2][0], rep.bundl.viewrep.map_matrix[3][0]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.map_matrix[0][1], rep.bundl.viewrep.map_matrix[1][1], rep.bundl.viewrep.map_matrix[2][1], rep.bundl.viewrep.map_matrix[3][1]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.map_matrix[0][2], rep.bundl.viewrep.map_matrix[1][2], rep.bundl.viewrep.map_matrix[2][2], rep.bundl.viewrep.map_matrix[3][2]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.map_matrix[0][3], rep.bundl.viewrep.map_matrix[1][3], rep.bundl.viewrep.map_matrix[2][3], rep.bundl.viewrep.map_matrix[3][3]);
#endif
  }
}

