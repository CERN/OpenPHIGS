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
 * \file pslss.c
 * \brief set light source state
 */
FTN_SUBROUTINE(pslss)(
                      FTN_INTEGER(nacti),
                      FTN_INTEGER_ARRAY(acti),
                      FTN_INTEGER(ndeacti),
                      FTN_INTEGER_ARRAY(deacti)
                      )
{
  Pint n_active = FTN_INTEGER_GET(nacti);
  Pint n_deactive = FTN_INTEGER_GET(ndeacti);
  Pint_list active;
  Pint_list deactive;
  Pint i;
  Pint aarr[n_active];
  Pint darr[n_deactive];
  for (i=0; i<n_active; i++){
    aarr[i] = FTN_INTEGER_ARRAY_GET(acti, i);
  }
  for (i=0; i<n_deactive; i++){
    darr[i] = FTN_INTEGER_ARRAY_GET(deacti, i);
  }
#ifdef DEBUG
  printf("DEBUG: set light source status elem 0 are active %d decactive %d\n", acti[0], deacti[0]);
#endif
  active.num_ints = n_active;
  active.ints = &aarr[0];
  deactive.num_ints = n_deactive;
  deactive.ints = &darr[0];
  pset_light_src_state(&active, &deactive);
}

