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
 * \file pslmt3.c
 * \brief SET LOCAL TRANSFORMATION 3
 */
FTN_SUBROUTINE(pslmt3)(
                       float * xfrmt,
                       FTN_INTEGER(ctype)
                       )
{
  Phg_args_add_el args;
  Pint *data;
  int i, j;
  Pmatrix3 x;
#ifdef DEBUG
  printf("DEBUG: pslmt3 called\n");
#endif
  for (i=0; i<4; i++){
    for (j=0; j<4; j++){
      x[j][i] = xfrmt[4*i+j];
    }
  }
  if (phg_entry_check(PHG_ERH, 0, Pfn_set_local_tran3)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_LOCAL_MODEL_TRAN3;
      args.el_size = sizeof(Pint) + 16 * sizeof(Pfloat);
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = (Pint) FTN_INTEGER_GET(ctype);
        phg_mat_unpack((Pfloat *) &data[1], x);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}
