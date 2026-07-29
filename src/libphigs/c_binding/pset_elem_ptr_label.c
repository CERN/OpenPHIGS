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
 * \file pset_elem_ptr_label.c
 * \brief Searches forward in the structure list for the specified label, and makes that the current element.  Search begins with the element after (to the right) of the current one. Question: should I include the current element in the search? Or does the search begin with the first element after the current one? This version gives fatal error if label not found.
 */
void pset_elem_ptr_label(
                         Pint label_id
                         )
{
  Phg_args_set_el_ptr args;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_elem_ptr_label)) {
    if (PSL_STRUCT_STATE(PHG_PSL) == PSTRUCT_ST_STOP) {
      args.op = PHG_ARGS_SETEP_LABEL;
      args.data = label_id;
      phg_set_el_ptr(PHG_CSS, &args);
    }
    else {
      ERR_REPORT(PHG_ERH, ERR5);
    }
  }
}

