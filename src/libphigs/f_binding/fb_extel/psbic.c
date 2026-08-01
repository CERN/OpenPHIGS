/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2022-2023 CERN
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <util/ftn.h>

#ifndef  MAX_ARRAY_SIZE
#define  MAX_ARRAY_SIZE 400
#endif

/**
 * \file psbic.c
 * \brief set back interior color
 */
FTN_SUBROUTINE(psbic)(
                      FTN_INTEGER(ityp),
                      FTN_INTEGER(incc),
                      FTN_INTEGER(icol),
                      FTN_REAL_ARRAY(rcolr)
                      )
{
  Pint colr_typ = FTN_INTEGER_GET(ityp);
  Pint colr_ind = FTN_INTEGER_GET(icol);
  Pint ncc = FTN_INTEGER_GET(incc);
  Pgcolr colr;
#ifdef DEBUG
  printf("DEBUG: pset interior color index set to %d\n", colr_ind);
#endif
  switch (colr_typ) {
  case PINDIRECT:
    colr.type = PINDIRECT;
    colr.val.ind = colr_ind;
    break;
  case PMODEL_RGB:
    colr.type = PMODEL_RGB;
    if (ncc == 3){
      colr.val.general.x = FTN_REAL_ARRAY_GET(rcolr, 0);
      colr.val.general.y = FTN_REAL_ARRAY_GET(rcolr, 1);
      colr.val.general.z = FTN_REAL_ARRAY_GET(rcolr, 2);
      colr.val.general.a = 1.0;
    }
    else {
      printf("PSBCI: not enough color values provided. Ignoring function.\n");
    }
    break;
  case PMODEL_RGBA:
    colr.type = PMODEL_RGBA;
    if (ncc == 4){
      colr.val.general.x = FTN_REAL_ARRAY_GET(rcolr, 0);
      colr.val.general.y = FTN_REAL_ARRAY_GET(rcolr, 1);
      colr.val.general.z = FTN_REAL_ARRAY_GET(rcolr, 2);
      colr.val.general.a = FTN_REAL_ARRAY_GET(rcolr, 3);
    }
    else {
      printf("PSBCI: not enough color values provided. Ignoring function.\n");
    }
    break;
  default:
    printf("PSBCI: Unknown color mode given. Ignoring function.\n");
    break;
  }
  pset_back_int_colr(&colr);
}

