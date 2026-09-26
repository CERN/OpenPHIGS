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
#include "private/cbP.h"

/**
 * \file pdel_struct.c
 *
 * \brief       Delete structure
 */
void pdel_struct(
                 Pint struct_id
                 )
{
  Struct_handle exists;
#ifdef DEBUG
  printf("Deleting structure with ID %d\n", struct_id);
#endif
  if (phg_entry_check(PHG_ERH, ERR2, Pfn_del_struct)) {
    exists = CSS_STRUCT_EXISTS(PHG_CSS, struct_id);
    if (exists != NULL){
      phg_del_struct(PHG_CSS, struct_id);
#ifdef VERBOSE
    } else {
      printf("ERROR: pdel_struct: Cannot delete structure %d.\n", struct_id);
      ERR_REPORT(PHG_ERH, ERR200);
#endif
    }
  } else {
    ERR_REPORT(PHG_ERH, ERR2);
  }
}
