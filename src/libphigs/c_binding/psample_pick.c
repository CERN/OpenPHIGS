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
 * \file psample_pick.c
 * \brief Sample pick device
 */
void psample_pick(
                  Pint ws_id,
                  Pint pick_num,
                  Pint depth,
                  Pin_status *pick_in_status,
                  Ppick_path *pick
                  )
{
  Phg_ret ret;
  Wst_input_wsdt *idt;
  Ppick *pik;
  Pint depth_limit;

  idt = input_ws_open(ws_id, Pfn_sample_pick, NULL, NULL);
  if (idt != NULL) {
    if ((pick_num > 0) &&  (pick_num <= idt->num_devs.pick)) {
      sample_device(ws_id, pick_num, PHG_ARGS_INP_PIK, &ret);
      if (ret.err == 0) {
        pik = &ret.data.inp_event.data.pik;
        *pick_in_status = pik->status;
        if (pik->status == PIN_STATUS_OK) {
          pick->depth = pik->pick_path.depth;
          depth_limit = PHG_MIN(depth, pik->pick_path.depth);
          if (depth_limit > 0) {
            memcpy(pick->path_list,
                   pik->pick_path.path_list,
                   depth_limit * sizeof(Ppick_path_elem));
          }
        }
      }
      else {
        *pick_in_status = PIN_STATUS_NONE;
      }
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

