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
#include "util.h"

/**
 * \file request_device.c
 * \brief Request device helper function
 */
void request_device(
                           Pint ws_id,
                           Pint dev_num,
                           Phg_args_idev_class dev_class,
                           Phg_ret *ret
                           )
{
  Pin_status in_status;

  /* The calling function shall always check the requested workstation first */
  Ws_handle wsh = PHG_WSID(ws_id);
  Ws_inp_req *inp = &wsh->in_ws.input_request;
  Phg_ret_inp_request *req = &ret->data.inp_request;

#ifdef DEBUGINP
  printf("now in request_device\n");
#endif
  (*wsh->request_device)(wsh, dev_class, dev_num, ret);

#ifdef DEBUGINP
  printf("Entering loop...\n");
#endif
  do {
    while (phg_wsx_input_dispatch_next(wsh, PHG_EVT_TABLE));

    switch (dev_class) {
    case PHG_ARGS_INP_LOC:
    case PHG_ARGS_INP_LOC3:
    case PHG_ARGS_INP_STK:
    case PHG_ARGS_INP_STK3:
    case PHG_ARGS_INP_VAL:
    case PHG_ARGS_INP_VAL3:
    case PHG_ARGS_INP_STR:
    case PHG_ARGS_INP_STR3:
      in_status = inp->status.istat;
      break;

    case PHG_ARGS_INP_PIK:
    case PHG_ARGS_INP_PIK3:
      in_status = inp->status.pkstat;
      break;

    case PHG_ARGS_INP_CHC:
    case PHG_ARGS_INP_CHC3:
      in_status = inp->status.chstat;
      break;
    }

    phg_msleep(1);

  } while ((in_status == PIN_STATUS_NONE) &&
           (inp->dev_class != dev_class) &&
           (inp->dev_num != dev_num));

  /* Copy to return argument */
  req->status.istat = in_status;
#ifdef DEBUGINP
  printf("Request device gives: %d\n", in_status);
#endif
  if (in_status != PIN_STATUS_NO_IN) {
    switch (dev_class) {
    case PHG_ARGS_INP_LOC:
    case PHG_ARGS_INP_LOC3:
      memcpy(&req->event.data.loc,
             &wsh->in_ws.input_request.evt.loc,
             sizeof(Ploc3));
      break;

    case PHG_ARGS_INP_STK:
         case PHG_ARGS_INP_STK3:
           memcpy(&req->event.data.stk,
                  &wsh->in_ws.input_request.evt.stroke,
                  sizeof(Pstroke3));
           break;

    case PHG_ARGS_INP_PIK:
    case PHG_ARGS_INP_PIK3:
      memcpy(&req->event.data.pik,
             &wsh->in_ws.input_request.evt.pick,
             sizeof(Ppick));
      req->status.pkstat = wsh->in_ws.input_request.status.pkstat;
#ifdef DEBUGINP
      printf("Pick event copy results %d -> %d\n",
	     wsh->in_ws.input_request.status.pkstat,
	     req->event.data.pik.status);
#endif
      break;

    case PHG_ARGS_INP_VAL:
    case PHG_ARGS_INP_VAL3:
      memcpy(&req->event.data.val,
             &wsh->in_ws.input_request.evt.val,
             sizeof(Pfloat));
      break;

    case PHG_ARGS_INP_CHC:
    case PHG_ARGS_INP_CHC3:
      memcpy(&req->event.data.chc,
             &wsh->in_ws.input_request.evt.choice,
             sizeof(Pchoice));
      break;

    case PHG_ARGS_INP_STR:
    case PHG_ARGS_INP_STR3:
      memcpy(&req->event.data.str,
             &wsh->in_ws.input_request.evt.string,
             sizeof(Phg_string));
      break;
    }
  }

  memset(inp, 0, sizeof(Ws_inp_req));
  ret->err = 0;
#ifdef DEBUGINP
  printf("Set ret->err to zero\n");
#endif
}

