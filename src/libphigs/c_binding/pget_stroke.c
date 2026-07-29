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
 * \file pget_stroke.c
 * \brief Get stroke event from event queue
 */
void pget_stroke(
                 Pint *view_ind,
                 Ppoint_list *stroke
                 )
{
  int i;
  Pstroke3 *stk;

  if (check_event_class(PIN_STROKE, Pfn_get_stroke)) {
    stk = &PSL_CUR_EVENT_DATA(PHG_PSL, stk);
    *view_ind = stk->view_ind;
    stroke->num_points = stk->num_points;
    for (i = 0; i < stk->num_points; i++) {
      stroke->points[i].x = stk->points[i].x;
      stroke->points[i].y = stk->points[i].y;
    }
  }
}

