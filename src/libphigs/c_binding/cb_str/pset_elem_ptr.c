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
 * \file pset_elem_ptr.c
 * \brief Make the requested index the "currently active element". NOTE: 0 means set element to even before the very first element; 1 means set elptr to very first element.
 */
void pset_elem_ptr(
                   Pint elem_ptr_value
                   )
{
  Phg_args_set_el_ptr args;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_elem_ptr)) {
    if (PSL_STRUCT_STATE(PHG_PSL) == PSTRUCT_ST_STOP) {
      args.op = PHG_ARGS_SETEP_ABS;
      args.data = elem_ptr_value;
      phg_set_el_ptr(PHG_CSS, &args);
    }
    else {
      ERR_REPORT(PHG_ERH, ERR5);
    }
  }
}

