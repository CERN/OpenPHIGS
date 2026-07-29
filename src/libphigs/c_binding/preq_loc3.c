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
 * \file preq_loc3.c
 * \brief Request input from locator device 3D
 */
void preq_loc3(
               Pint ws_id,
               Pint loc_num,
               Pin_status *in_status,
               Pint *view_ind,
               Ppoint3 *loc_pos
               )
{
  Wst_input_wsdt *idt;
  Phg_ret ret;
  Phg_ret_inp_request *req = &ret.data.inp_request;

  idt = input_ws_open(ws_id, Pfn_req_loc3, NULL, NULL);
  if (idt != NULL) {
    if ((loc_num > 0) && (loc_num <= idt->num_devs.loc)) {
      request_device(ws_id, loc_num, PHG_ARGS_INP_LOC3, &ret);
      if (ret.err == 0) {
        *in_status = req->status.istat;
        if (req->status.istat != PIN_STATUS_NO_IN) {
          *view_ind = req->event.data.loc.view_ind;
          memcpy(loc_pos,
                 &req->event.data.loc.position,
                 sizeof(Ppoint3));
        }
      }
      else {
        *in_status = PIN_STATUS_NO_IN;
      }
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

