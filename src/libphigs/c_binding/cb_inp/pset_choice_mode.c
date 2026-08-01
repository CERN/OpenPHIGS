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
 * \file pset_choice_mode.c
 * \brief Set choice input device mode
 */
void pset_choice_mode(
                      Pint ws_id,
                      Pint choice_dev,
                      Pop_mode op_mode,
                      Pecho_switch echo_switch
                      )
{
  Wst_input_wsdt *idt;

  idt = input_ws_open(ws_id, Pfn_set_choice_mode, NULL, NULL);
  if (idt != NULL) {
    if (choice_dev > 0) {
      set_mode(ws_id, PHG_ARGS_INP_CHC, choice_dev, op_mode, echo_switch);
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

