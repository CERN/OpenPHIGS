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
 * \file pads.c
 * \brief add names to set
 */

FTN_SUBROUTINE(pads)(
                     FTN_INTEGER(n),
                     FTN_INTEGER_ARRAY(nameset)
                     )
{
  Pint num_names = FTN_INTEGER_GET(n);
  Pint arr[num_names];
  int i;
#ifdef DEBUG
  printf("DEBUG: adding %d names\n", num_names);
#endif
  for (i=0; i<num_names; i++){
    arr[i] =  FTN_INTEGER_ARRAY_GET(nameset, i);
  }
  Pint_list list = {num_names, arr};
  padd_names_set(&list);
}

