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
#include <string.h>

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "private/cbP.h"
#include "util/ftn.h"

/**
 * \file dumpstr.c
 * \brief Print structure as string
 */
FTN_SUBROUTINE(dumpstr)(FTN_INTEGER(strid)){
  Struct_handle structp;
  Pint struct_id = FTN_INTEGER_GET(strid);
  if ( !(structp = CSS_STRUCT_EXISTS(PHG_CSS, struct_id)) ) {
    printf("Could not find struct_id %d\n", struct_id);
  } else {
    printf("Dumping structure %d\n", struct_id);
    phg_css_print_struct(structp, 0);
  }
}

