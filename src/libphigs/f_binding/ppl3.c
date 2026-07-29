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
#include "util/ftn.h"

/**
 * \file ppl3.c
 * \brief Creates a new element - Polyline 3
 */
FTN_SUBROUTINE(ppl3)(
                     FTN_INTEGER(n),
                     FTN_REAL_ARRAY(pxa),
                     FTN_REAL_ARRAY(pya),
                     FTN_REAL_ARRAY(pza)
                     )
{
  Pint num_points = FTN_INTEGER_GET(n);
  Phg_args_add_el args;
  Pint i;
  Pint  *data;
  Ppoint3 *point;

#ifdef DEBUG
  printf("DEBUG: PPL3 pset polyline3 called\n");
#endif
  if (phg_entry_check(PHG_ERH, 0, Pfn_polyline3)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_POLYLINE3;
      args.el_size = sizeof(Pint) + sizeof(Ppoint3) * num_points;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = (Pint) num_points;
        point = (Ppoint3*) &data[1];
        for (i = 0; i < num_points; i++) {
          point[i].x = FTN_REAL_ARRAY_GET(pxa, i);
          point[i].y = FTN_REAL_ARRAY_GET(pya, i);
          point[i].z = FTN_REAL_ARRAY_GET(pza, i);
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

