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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cb_internal.h"

/**
 * \file pfill_area_set3.c
 * \brief Creates a new element - Fill area set 3D
 */
void pfill_area_set3(
                     Ppoint_list_list3 *point_list_list
                     )
{
  Phg_args_add_el args;
  Pint i, num_points;
  Pint num_lists;
  Pint *data;
  Ppoint3 *pts;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_fill_area_set3)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_FILL_AREA_SET3;
      num_lists = point_list_list->num_point_lists;
      for (i = 0, num_points = 0; i < num_lists; i++) {
        num_points += point_list_list->point_lists[i].num_points;
      }
      args.el_size = sizeof(Pint) +
        sizeof(Pint) * num_lists + sizeof(Ppoint3) * num_points;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = num_lists;
        data = &data[1];
        for (i = 0; i < num_lists; i++) {
          data[0] = point_list_list->point_lists[i].num_points;
          pts = (Ppoint3 *) &data[1];
          memcpy(pts, point_list_list->point_lists[i].points,
                 data[0] * sizeof(Ppoint3));
          data = (Pint *) &pts[data[0]];
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

