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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <private/wsxP.h>
#include <util/ftn.h>

/**
 * \file psmlc3.c
 * \brief sample locator 3
 *
 * \return view index, current point in WC
 */
FTN_SUBROUTINE(psmlc3)(
                       FTN_INTEGER(wkid),
                       FTN_INTEGER(lcdnr),
                       int *viewi,
                       float *lpx,
                       float *lpy,
                       float *lpz
                       )
{
  Pint    ws_id = FTN_INTEGER_GET(wkid);
  Pint    loc_dev = FTN_INTEGER_GET(lcdnr);
  Ppoint3 locpos;
  psample_loc3(ws_id, loc_dev, viewi, &locpos);
  *lpx = locpos.x;
  *lpy = locpos.y;
  *lpz = locpos.z;
}

