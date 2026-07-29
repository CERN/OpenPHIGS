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

