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
 * \file psample_string.c
 * \brief Sample string device
 */
void psample_string(
                    Pint ws_id,
                    Pint string_dev,
                    char* string
                    )
{
  Phg_ret ret;
  Wst_input_wsdt *idt;

  idt = input_ws_open(ws_id, Pfn_sample_pick, NULL, NULL);
  if (idt != NULL) {
    if (string_dev > 0) {
      sample_device(ws_id, string_dev, PHG_ARGS_INP_STR, &ret);
      if (ret.err == 0) {
        strncpy(string, ret.data.inp_event.data.str.string, ret.data.inp_event.data.str.length);
      }
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

