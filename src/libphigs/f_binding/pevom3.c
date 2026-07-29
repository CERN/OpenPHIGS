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
 * \file pevom3.c
 * \brief Evaluate view orientation matrix 3
 *
 * \return Error index, orientation matrix
 */
FTN_SUBROUTINE(pevom3)(
                       FTN_REAL(vwrx), FTN_REAL(vwry), FTN_REAL(vwrz),
                       FTN_REAL(vpnx), FTN_REAL(vpny), FTN_REAL(vpnz),
                       FTN_REAL(vupx), FTN_REAL(vupy), FTN_REAL(vupz),
                       int* err_ind, Pfloat * vwormt
                       )
{
  Ppoint3 vrp;
  Pvec3 vpn;
  Pvec3 vup;
  int i, j;
  Pmatrix3 x;
#ifdef DEBUG
  printf("DEBUG: pevom3 called\n");
#endif
  vrp.x = FTN_REAL_GET(vwrx);
  vrp.y = FTN_REAL_GET(vwry);
  vrp.z = FTN_REAL_GET(vwrz);

  vpn.delta_x = FTN_REAL_GET(vpnx);
  vpn.delta_y = FTN_REAL_GET(vpny);
  vpn.delta_z = FTN_REAL_GET(vpnz);

  vup.delta_x = FTN_REAL_GET(vupx);
  vup.delta_y = FTN_REAL_GET(vupy);
  vup.delta_z = FTN_REAL_GET(vupz);

  peval_view_ori_matrix3(&vrp, &vpn, &vup, err_ind, x);
  for (i=0; i<4; i++){
    for (j=0; j<4; j++){
      vwormt[4*j+i] = (float)x[i][j];
    }
  }
#ifdef DEBUG
  printf("DEBUG: pevom3 returned %d\n", *err_ind);
  if (*err_ind == 0){
    printf("Resulting matrix:");
    printf("    %f %f %f %f\n", vwormt[0], vwormt[1], vwormt[2], vwormt[3]    );
    printf("    %f %f %f %f\n", vwormt[4], vwormt[5], vwormt[6], vwormt[7]    );
    printf("    %f %f %f %f\n", vwormt[8], vwormt[9], vwormt[10],vwormt[11]  );
    printf("    %f %f %f %f\n", vwormt[12],vwormt[13],vwormt[14],vwormt[15]);
  }
#endif
}

