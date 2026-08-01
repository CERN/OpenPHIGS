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

#include "phg.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "private/sinqP.h"
#include "private/wsxP.h"

/**
 * \file psample_loc3.c
 * \brief Sample locator device 3D
 */
void psample_loc3(
                  Pint ws_id,
                  Pint loc_num,
                  Pint *view_ind,
                  Ppoint3 *loc_pos
                  )
{
  Phg_ret ret;
  Wst_input_wsdt *idt;

  idt = input_ws_open(ws_id, Pfn_sample_loc3, NULL, NULL);
  if (idt != NULL) {
    if ((loc_num > 0) &&  (loc_num <= idt->num_devs.loc)) {
      sample_device(ws_id, loc_num, PHG_ARGS_INP_LOC3, &ret);
      if (ret.err == 0) {
        *view_ind = ret.data.inp_event.data.loc.view_ind;
        memcpy(loc_pos,
               &ret.data.inp_event.data.loc.position,
               sizeof(Ppoint3));
      }
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

