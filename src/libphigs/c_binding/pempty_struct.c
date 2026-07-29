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
 * \file pempty_struct.c
 * \brief Clear structure
 */
void pempty_struct(
                   Pint struct_id
                   )
{
  Phg_args_del_el args;

  if (phg_entry_check(PHG_ERH, ERR2, Pfn_empty_struct)) {
    args.op = PHG_ARGS_EMPTY_STRUCT;
    args.data.struct_id = struct_id;
    phg_del_el(PHG_CSS, &args);
  }
}

