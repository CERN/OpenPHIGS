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
 * \file psample_stroke3.c
 * \brief Sample stroke device 3D
 */
void psample_stroke3(
                     Pint ws_id,
                     Pint stroke_num,
                     Pint *view_ind,
                     Ppoint_list3 *stroke
                     )
{
  Phg_ret ret;
  Wst_input_wsdt *idt;
  Pstroke3 *stk;

  idt = input_ws_open(ws_id, Pfn_sample_stroke3, NULL, NULL);
  if (idt != NULL) {
    if ((stroke_num > 0) &&  (stroke_num <= idt->num_devs.stroke)) {
      sample_device(ws_id, stroke_num, PHG_ARGS_INP_STK3, &ret);
      if (ret.err == 0) {
        stk = &ret.data.inp_event.data.stk;
        *view_ind = stk->view_ind;
        stroke->num_points = stk->num_points;
        if (stk->num_points > 0) {
          memcpy(&stroke->points,
                 &stk->points,
                 stk->num_points * sizeof(Ppoint3));
        }
        else {
          stroke->num_points = 0;
        }
      }
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

