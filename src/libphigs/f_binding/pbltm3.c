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
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <util/ftn.h>

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(pbltm3)(;
extern FTN_SUBROUTINE(pevmm3)(;
extern FTN_SUBROUTINE(pevom3)(;
extern FTN_SUBROUTINE(pslmt3)(;


/**
 * \file pbltm3.c
 * \brief Build transformation matrix 3
 *
 * \return error index, transformation matrix
 */
FTN_SUBROUTINE(pbltm3)(
                       FTN_REAL(xo), FTN_REAL(yo), FTN_REAL(zo),
                       FTN_REAL(dx), FTN_REAL(dy), FTN_REAL(dz),
                       FTN_REAL(phix), FTN_REAL(phiy), FTN_REAL(phiz),
                       FTN_REAL(fx), FTN_REAL(fy), FTN_REAL(fz),
                       int* err_ind, Pfloat *xfrmt
                       )
{
  Ppoint3 opoint;
  Pvec3 dpoint;
  Pvec3 fpoint;
  int i, j;
  Pmatrix3 x;

#ifdef DEBUG
  printf("DEBUG: pbltm3 called\n");
#endif
  opoint.x = FTN_REAL_GET(xo);
  opoint.y = FTN_REAL_GET(yo);
  opoint.z = FTN_REAL_GET(zo);

  dpoint.delta_x = FTN_REAL_GET(dx);
  dpoint.delta_y = FTN_REAL_GET(dy);
  dpoint.delta_z = FTN_REAL_GET(dz);

  fpoint.delta_x = FTN_REAL_GET(fx);
  fpoint.delta_y = FTN_REAL_GET(fy);
  fpoint.delta_z = FTN_REAL_GET(fz);

  pbuild_tran_matrix3(&opoint, &dpoint,
                      FTN_REAL_GET(phix),
                      FTN_REAL_GET(phiy),
                      FTN_REAL_GET(phiz),
                      &fpoint,
                      err_ind, x);
  for (i=0; i<4; i++){
    for (j=0; j<4; j++){
      xfrmt[4*j+i] = (float)x[i][j];
    }
  }
#ifdef DEBUG
  printf("DEBUG: pbltm3 returned %d\n", *err_ind);
  if (*err_ind == 0){
    printf("Resulting matrix:");
    printf("    %f %f %f %f\n", xfrmt[0],xfrmt[1],xfrmt[2],xfrmt[3]    );
    printf("    %f %f %f %f\n", xfrmt[4],xfrmt[5],xfrmt[6],xfrmt[7]    );
    printf("    %f %f %f %f\n", xfrmt[8],xfrmt[9],xfrmt[10],xfrmt[11]  );
    printf("    %f %f %f %f\n", xfrmt[12],xfrmt[13],xfrmt[14],xfrmt[15]);
  }
#endif
}

