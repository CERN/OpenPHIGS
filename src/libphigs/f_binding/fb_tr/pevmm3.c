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

/**
 * \file pevmm3.c
 * \brief EVALUATE VIEW MAPPING MATRIX 3
 *
 * \return error index, view mapping matrix
 */
FTN_SUBROUTINE(pevmm3)(
                       FTN_REAL_ARRAY(vwwnlm),
                       FTN_REAL_ARRAY(pjvplm),
                       FTN_INTEGER(pjtype),
                       FTN_REAL(pjrx),
                       FTN_REAL(pjry),
                       FTN_REAL(pjrz),
                       FTN_REAL(vp),
                       FTN_REAL(bp),
                       FTN_REAL(fp),
                       int * err_ind,
                       Pfloat * vwmpmt
                       )
{
  Pproj_type proj_type;
  Pview_map3 map;
  int i, j;
  Pmatrix3 x;
#ifdef DEBUG
  printf("DEBUG: pevmm3 called\n");
#endif

  map.win.x_min = FTN_REAL_ARRAY_GET(vwwnlm, 0);
  map.win.x_max = FTN_REAL_ARRAY_GET(vwwnlm, 1);
  map.win.y_min = FTN_REAL_ARRAY_GET(vwwnlm, 2);
  map.win.y_max = FTN_REAL_ARRAY_GET(vwwnlm, 3);
  map.win.z_min = 0;
  map.win.z_max = 0;

  map.proj_vp.x_min = FTN_REAL_ARRAY_GET(pjvplm, 0);
  map.proj_vp.x_max = FTN_REAL_ARRAY_GET(pjvplm, 1);
  map.proj_vp.y_min = FTN_REAL_ARRAY_GET(pjvplm, 2);
  map.proj_vp.y_max = FTN_REAL_ARRAY_GET(pjvplm, 3);
  map.proj_vp.z_min = FTN_REAL_ARRAY_GET(pjvplm, 4);
  map.proj_vp.z_max = FTN_REAL_ARRAY_GET(pjvplm, 5);

  map.proj_type = FTN_INTEGER_GET(pjtype);

  map.proj_ref_point.x = FTN_REAL_GET(pjrx);
  map.proj_ref_point.y = FTN_REAL_GET(pjry);
  map.proj_ref_point.z = FTN_REAL_GET(pjrz);

  map.view_plane = FTN_REAL_GET(vp);
  map.back_plane = FTN_REAL_GET(bp);
  map.front_plane = FTN_REAL_GET(fp);

  peval_view_map_matrix3(&map, err_ind, x);
  for (i=0; i<4; i++){
    for (j=0; j<4; j++){
      vwmpmt[4*j+i] = (float)x[i][j];
    }
  }

#ifdef DEBUG
  printf("DEBUG: pevmm3 returned %d\n", *err_ind);
  if (*err_ind == 0){
    printf("Resulting matrix:");
    printf("    %f %f %f %f\n", vwmpmt[0],vwmpmt[1],vwmpmt[2],vwmpmt[3]    );
    printf("    %f %f %f %f\n", vwmpmt[4],vwmpmt[5],vwmpmt[6],vwmpmt[7]    );
    printf("    %f %f %f %f\n", vwmpmt[8],vwmpmt[9],vwmpmt[10],vwmpmt[11]  );
    printf("    %f %f %f %f\n", vwmpmt[12],vwmpmt[13],vwmpmt[14],vwmpmt[15]);
  }
#endif
}

