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
 * \file psatcu.c
 * \brief set annotation text character up vector
 */

FTN_SUBROUTINE(psatcu)(
                       FTN_REAL(atchux),
                       FTN_REAL(atchuy)
                       )
{
#ifdef DEBUG
  printf("DEBUG: PSATCU pset char up vector called\n");
#endif
  Pvec char_up_vec;
  char_up_vec.delta_x = FTN_REAL_GET(atchux);
  char_up_vec.delta_y = FTN_REAL_GET(atchuy);
  pset_anno_char_up_vec(&char_up_vec);
}

