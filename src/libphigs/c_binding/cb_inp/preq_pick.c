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
 * \file preq_pick.c
 * \brief Request input from stroke device 3D
 */
void preq_pick(
               Pint ws_id,
               Pint pick_num,
               Pint depth,
               Pin_status *status,
               Ppick_path *pick
               )
{
  Wst_input_wsdt *idt;
  Phg_ret ret;
  Phg_ret_inp_request *req = &ret.data.inp_request;

  idt = input_ws_open(ws_id, Pfn_req_pick, NULL, NULL);
  if (idt != NULL) {
    if ((pick_num > 0) && (pick_num <= idt->num_devs.loc)) {
      request_device(ws_id, pick_num, PHG_ARGS_INP_PIK, &ret);
      if (ret.err == 0) {
        *status = req->status.pkstat;
#ifdef DEBUGINP
	printf("Request pick: status %d\n", *status);
	printf("  pick status:       %d\n", req->status.pkstat);
#endif
        if (req->status.pkstat != PIN_STATUS_NO_IN) {
          *status = req->event.data.pik.status;
          memcpy(pick, &req->event.data.pik.pick_path, sizeof(Ppick_path));
        }
      }
    }
  }
}

