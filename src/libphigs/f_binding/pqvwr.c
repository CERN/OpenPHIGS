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
 * \file pqvwr.c
 * \brief Inquire view representation
 *
 * \return error index, number of entries, Nth element of defined view indices
 */

FTN_SUBROUTINE(pqvwr)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(viewi),
                      FTN_INTEGER(curq),
                      FTN_INTEGER(err_ind),
                      int* vwupd,
                      Pfloat* vwormt,
                      Pfloat* vwmpmt,
                      Pfloat* vwcplm ,
                      int * xyclip,
                      int* bclip,
                      int* fclip
                      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint index = FTN_INTEGER_GET(viewi);
  Pint itype = FTN_INTEGER_GET(curq);
  Pinq_type type;
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Phg_ret ret;
  int i, j;

#ifdef DEBUG
  printf("DEBUG: pqvwr request view for %d\n", ws_id);
#endif

  switch (itype) {
  case 0:
    type = PINQ_SET;
    break;
  case 1:
    type = PINQ_REALIZED;
    break;
  }
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
            dt->ws_category == PCAT_EPS ||
            dt->ws_category == PCAT_PDF ||
            dt->ws_category == PCAT_SVG ||
            dt->ws_category == PCAT_OBJ ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else if (index < 1) {
        *err_ind = ERR100;
      }
      else {
        wsh = PHG_WSID(ws_id);
        (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_VIEWREP, &ret);
        if (ret.err) {
          *err_ind = ret.err;
        } else {
          *err_ind = 0;
          if (ret.data.view_rep.update_state == PUPD_NOT_PEND){*vwupd = 0;} else {*vwupd = 1;};
          for (i=0;i<4;i++){
            for (j=0;j<4;j++){
              vwormt[4*j+i] = ret.data.rep.viewrep.ori_matrix[i][j];
              vwmpmt[4*j+i] = ret.data.rep.viewrep.map_matrix[i][j];
            }
          }
          vwcplm[0] =  ret.data.rep.viewrep.clip_limit.x_min;
          vwcplm[1] =  ret.data.rep.viewrep.clip_limit.x_max;
          vwcplm[2] =  ret.data.rep.viewrep.clip_limit.y_min;
          vwcplm[3] =  ret.data.rep.viewrep.clip_limit.y_max;
          vwcplm[4] =  ret.data.rep.viewrep.clip_limit.z_min;
          vwcplm[5] =  ret.data.rep.viewrep.clip_limit.z_max;
          if (ret.data.rep.viewrep.xy_clip)   {*xyclip = 1;} else {*xyclip = 0;};
          if (ret.data.rep.viewrep.back_clip) {*bclip = 1;}  else {*bclip = 0;};
          if (ret.data.rep.viewrep.front_clip){*fclip = 1;}  else {*fclip = 0;};
        }
      }
    }
  }
#ifdef DEBUG
  if (*err_ind == 0){
    printf("VWORMT:");
    printf("    %f %f %f %f\n", vwormt[0],vwormt[1],vwormt[2],vwormt[3]);
    printf("    %f %f %f %f\n", vwormt[4],vwormt[5],vwormt[6],vwormt[7]);
    printf("    %f %f %f %f\n", vwormt[8],vwormt[9],vwormt[10],vwormt[11]);
    printf("    %f %f %f %f\n", vwormt[12],vwormt[13],vwormt[14],vwormt[15]);
    printf("VWMPMT:");
    printf("    %f %f %f %f\n", vwmpmt[0],vwmpmt[1],vwmpmt[2],vwmpmt[3]);
    printf("    %f %f %f %f\n", vwmpmt[4],vwmpmt[5],vwmpmt[6],vwmpmt[7]);
    printf("    %f %f %f %f\n", vwmpmt[8],vwmpmt[9],vwmpmt[10],vwmpmt[11]);
    printf("    %f %f %f %f\n", vwmpmt[12],vwmpmt[13],vwmpmt[14],vwmpmt[15]);
  } else {
    printf("Error in pqvwr! %d\n", *err_ind);
  }
#endif
}

