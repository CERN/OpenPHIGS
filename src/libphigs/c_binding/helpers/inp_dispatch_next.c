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
 * \file inp_dispatch_next.c
 * \brief Dispatch next event for all open input workstations
 *
 * \return TRUE or FALSE
 */
int inp_dispatch_next(
                      Pint fn_id
                      )
{
  Pint i, err_ind;
  Wst_input_wsdt *idt;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  int status = FALSE;

  for (i = 0; i < MAX_NO_OPEN_WS; i++) {
    idt = input_ws_open(i, fn_id, &dt, &err_ind);
    if (idt != NULL) {
      if ((dt->ws_category == PCAT_IN) ||
          (dt->ws_category == PCAT_OUTIN)) {
        wsh = PHG_WSID(i);
        status = phg_wsx_input_dispatch_next(wsh, PHG_EVT_TABLE);
      }
    }
  }
  return status;
}

