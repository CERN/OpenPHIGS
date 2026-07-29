/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2022-2023 CERN
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <private/wsxP.h>
#include <util/ftn.h>

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(pprec)(;
extern FTN_SUBROUTINE(pinlc3)(;
extern FTN_SUBROUTINE(pspkm)(;
extern FTN_SUBROUTINE(pwait)(;
extern FTN_SUBROUTINE(pslcm)(;
extern FTN_SUBROUTINE(pinlc)(;
extern FTN_SUBROUTINE(pspkft)(;
extern FTN_SUBROUTINE(pshlft)(;
extern FTN_SUBROUTINE(pinpk)(;
extern FTN_SUBROUTINE(pgtpk)(;
extern FTN_SUBROUTINE(prqpk)(;
extern FTN_SUBROUTINE(psmpk)(;
extern FTN_SUBROUTINE(psmlc)(;
extern FTN_SUBROUTINE(psmlc3)(;
extern FTN_SUBROUTINE(prqlc3)(;
extern FTN_SUBROUTINE(psmst)(;
extern FTN_SUBROUTINE(psstm)(;
extern FTN_SUBROUTINE(pinst)(;
extern FTN_SUBROUTINE(pinst3)(;
extern FTN_SUBROUTINE(prqst)(;
extern FTN_SUBROUTINE(pinch3)(;
extern FTN_SUBROUTINE(pschm)(;
extern FTN_SUBROUTINE(pgtch)(;
extern FTN_SUBROUTINE(prqch)(;
extern FTN_SUBROUTINE(pinvl3)(;
extern FTN_SUBROUTINE(psvlm)(;
extern FTN_SUBROUTINE(pgtvl)(;
extern FTN_SUBROUTINE(prqvl)(;
extern FTN_SUBROUTINE(pflush)(;
extern FTN_SUBROUTINE(pqdlc3)(;


/**
 * \file prqpk.c
 * \brief request pick
 *
 * \return state, depth, path
 */
FTN_SUBROUTINE(prqpk)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(pkdnr),
                      FTN_INTEGER(ippd),
                      Pint *stat,
                      Pint *ppd,
                      Pint *pp
                      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint pick_num = FTN_INTEGER_GET(pkdnr);
  Pint depth = FTN_INTEGER_GET(ippd);
  Ppick_path_elem  path_list[10];
  Ppick_path pick = {0, path_list};
  Pin_status status;
  Phg_ret ret;
  int i;
  preq_pick(ws_id, pick_num, depth, &status, &pick);
  *stat = (int)status;
  if ( status == PIN_STATUS_OK) {
    *ppd = pick.depth;
    for (i=0; i<pick.depth; i++){
      pp[3*i] = pick.path_list[i].struct_id;
      pp[3*i+1] = pick.path_list[i].pick_id;
      pp[3*i+2] = pick.path_list[i].elem_pos;
    }
  }
}

