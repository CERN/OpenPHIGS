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
 * \file preq_string.c
 * \brief Request input from string
 */
void preq_string(
                 Pint ws_id,
                 Pint string_dev,
                 Pin_status *status,
                 char *string
                 )
{
  Wst_input_wsdt *idt;
  Phg_ret ret;
  Phg_ret_inp_request *req = &ret.data.inp_request;
  Pint length;
  idt = input_ws_open(ws_id, Pfn_req_string, NULL, NULL);
  if (idt != NULL) {
    if (string_dev > 0) {
#ifdef DEBUGINP
      printf("Calling request_device\n");
#endif
      request_device(ws_id, string_dev, PHG_ARGS_INP_STR, &ret);
#ifdef DEBUGINP
      printf("Request device returned %d\n", ret.err);
#endif
      if (ret.err == 0) {
        *status = req->status.istat;
        if (req->status.istat != PIN_STATUS_NO_IN) {
          length = req->event.data.str.length;
          memcpy(string, req->event.data.str.string, length);
          string[length]='\0';
        } else {
          string[0] = '\0';
        }
      }
    }
  }
}

