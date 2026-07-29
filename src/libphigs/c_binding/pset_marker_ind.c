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
 * \file pset_marker_ind.c
 * \brief Creates a new element - Marker Attribute Index
 */
void pset_marker_ind(
                     Pint marker_ind
                     )
{
  Phg_args_add_el args;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_marker_ind)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else if (marker_ind < 1) {
      ERR_REPORT(PHG_ERH, ERR100);
    }
    else {
      args.el_type = PELEM_MARKER_IND;
      args.el_size = sizeof(Pint);
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        memcpy(args.el_data, &marker_ind, args.el_size);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

