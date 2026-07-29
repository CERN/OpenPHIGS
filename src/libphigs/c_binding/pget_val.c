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
 * \file pget_val.c
 * \brief Get valuator event from event queue
 */
void pget_val(
              Pfloat *val
              )
{
  Pfloat value;
  if (PSL_CUR_EVENT_CLASS(PHG_PSL) != PIN_NONE){
    if (check_event_class(PIN_VAL, Pfn_get_val)) {
      value = PSL_CUR_EVENT_DATA(PHG_PSL, val);
    } else {
      value = 0.0;
    }
    *val = value;
  }
}

