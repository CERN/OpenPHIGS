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
 * \file psrfp.c
 * \brief set reflectance properties
 */
FTN_SUBROUTINE(psrfp)(
                      FTN_INTEGER(refl_type1),
                      FTN_INTEGER(refl_len1),
                      char* datarec
                      ){
  Pint refl_type = FTN_INTEGER_GET(refl_type1);
  int refl_len = FTN_INTEGER_GET(refl_len1);
  int* here = (int*)datarec;
  float* fp;
  Pfloat farr[4];
  Prefl_props refl_properties;
  int num_ints = here[0];
  if (refl_type == 1) {
    int col_type = here[1];
    int ncc = here[2];
    int index = here[3];
    refl_properties.specular_colr.type = col_type;
    fp = (float*) &here[5];
    refl_properties.ambient_coef = fp[0];
    refl_properties.diffuse_coef = fp[1];
    refl_properties.specular_coef = fp[2];
    refl_properties.specular_exp = fp[3];
#ifdef DEBUG
    printf("PSRFP: Reflectance %f %f %f %f",
           refl_properties.ambient_coef,
           refl_properties.diffuse_coef,
           refl_properties.specular_coef,
           refl_properties.specular_exp);
#endif
    switch (col_type){
    case  PINDIRECT:
      refl_properties.specular_colr.val.ind = index;
      break;
    case PMODEL_RGB:
      refl_properties.specular_colr.val.general.x = fp[4];
      refl_properties.specular_colr.val.general.y = fp[5];
      refl_properties.specular_colr.val.general.z = fp[6];
      refl_properties.specular_colr.val.general.a = 1.0;
      break;
    case PMODEL_RGBA:
      refl_properties.specular_colr.val.general.x = fp[4];
      refl_properties.specular_colr.val.general.y = fp[5];
      refl_properties.specular_colr.val.general.z = fp[6];
      refl_properties.specular_colr.val.general.z = fp[7];
      break;
    default:
      printf("ERROR in psrfp: unknown color model %d.", col_type);
    }
    pset_refl_props(&refl_properties);
  } else {
    printf("ERROR in psrfp: unknown reflection type. Ignorning function.\n");
  }
}

