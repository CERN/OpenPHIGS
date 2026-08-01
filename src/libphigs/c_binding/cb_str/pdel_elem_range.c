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
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "private/cbP.h"

/**
 * \file pdel_elem_range.c
 * \brief Deletes all elements within and on the bounds of the given range.  The element pointer is left pointing to the element just prior to the first element deleted.
 */
void pdel_elem_range(
                     Pint elem_ptr1_value,
                     Pint elem_ptr2_value
                     )
{
  Phg_args_del_el args;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_del_elem_range)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.op = PHG_ARGS_DEL_RANGE;
      args.data.ep_values.ep1 = PHG_MAX(0, elem_ptr1_value);
      args.data.ep_values.ep2 = elem_ptr2_value;
      phg_del_el(PHG_CSS, &args);
    }
  }
}

