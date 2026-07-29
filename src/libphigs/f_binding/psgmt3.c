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
 * \file psgmt3.c
 * \brief set global transformation 3
 */
FTN_SUBROUTINE(psgmt3)(
                       float* xfrmt
                       )
{
  Pmatrix3 global_tran;
  int i, j;
#ifdef DEBUG
  printf("DEBUG: PSGMT3 set global transformation matrix.\n");
#endif
  for (i=0; i<4; i++){
    for (j=0; j<4; j++){
      global_tran[j][i] = (Pfloat) xfrmt[4*i+j];
    }
  }
  pset_global_tran3(global_tran);
}

