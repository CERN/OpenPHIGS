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

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(pslss)(;
extern FTN_SUBROUTINE(psism)(;
extern FTN_SUBROUTINE(psbism)(;
extern FTN_SUBROUTINE(psrfm)(;
extern FTN_SUBROUTINE(psrfeq)(;
extern FTN_SUBROUTINE(psbrfm)(;
extern FTN_SUBROUTINE(psrfp)(;
extern FTN_SUBROUTINE(pfas3d)(;
extern FTN_SUBROUTINE(pfasd)(;
extern FTN_SUBROUTINE(psfcm)(;
extern FTN_SUBROUTINE(psfdm)(;
extern FTN_SUBROUTINE(psalch)(;
extern FTN_SUBROUTINE(psbis)(;
extern FTN_SUBROUTINE(psbic)(;
extern FTN_SUBROUTINE(psii)(;
extern FTN_SUBROUTINE(pres)(;
extern FTN_SUBROUTINE(psisi)(;
extern FTN_SUBROUTINE(psmcli)(;
extern FTN_SUBROUTINE(psmcv3)(;
extern FTN_SUBROUTINE(psbici)(;
extern FTN_SUBROUTINE(pbsi)(;
extern FTN_SUBROUTINE(psbisi)(;


/**
 * \file psii.c
 * \brief set interior index
 */
FTN_SUBROUTINE(psii)(
                     FTN_INTEGER(ii)
                     )
{
  Pint iindex = FTN_INTEGER_GET(ii);
  pset_int_ind(iindex);
}

