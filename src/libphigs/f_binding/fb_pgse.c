/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2022 CERN
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "util/ftn.h"

/*******************************************************************************
 * pxshlc
 *
 * DESCR:	Extension: set highlighting colour
 * RETURNS:	N/A
 */
FTN_SUBROUTINE(pxshlc)(
                       FTN_INTEGER(ctype),
                       FTN_INTEGER(ncc),
                       FTN_INTEGER(coli),
                       FTN_REAL_ARRAY(colr))
{
#ifdef DEBUG
  printf("pxshlc: set highlighting color\n");
#endif
  Pgcolr pgcolr;
  pgcolr.type = FTN_INTEGER_GET(ctype);
  switch (pgcolr.type){
  case PINDIRECT:
    pgcolr.val.ind = FTN_INTEGER_GET(coli);
    break;
  case PMODEL_RGB:
    pgcolr.val.general.x = FTN_REAL_ARRAY_GET(colr, 0);
    pgcolr.val.general.y = FTN_REAL_ARRAY_GET(colr, 1);
    pgcolr.val.general.z = FTN_REAL_ARRAY_GET(colr, 2);
    pgcolr.val.general.a = 1.0;
    break;
  case PMODEL_RGBA:
    pgcolr.val.general.x = FTN_REAL_ARRAY_GET(colr, 0);
    pgcolr.val.general.y = FTN_REAL_ARRAY_GET(colr, 1);
    pgcolr.val.general.z = FTN_REAL_ARRAY_GET(colr, 2);
    pgcolr.val.general.a = FTN_REAL_ARRAY_GET(colr, 3);
    break;
  default:
    printf("WARNING in pxshlc: Unknown color model %d. Ignoring function.\n", pgcolr.type);
    break;
  };
  pxset_highlight_colr(&pgcolr);
};
