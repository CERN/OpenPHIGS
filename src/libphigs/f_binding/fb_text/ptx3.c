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
#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "util/ftn.h"

/**
 * \file ptx3.c
 * \brief TEXT 3
 */
FTN_SUBROUTINE(ptx3)(
                     FTN_REAL(px),
                     FTN_REAL(py),
                     FTN_REAL(pz),
                     FTN_REAL_ARRAY(tdx),
                     FTN_REAL_ARRAY(tdy),
                     FTN_REAL_ARRAY(tdz),
                     FTN_CHARACTER(chars)
                     )
{

  Phg_args_add_el args;
  Pint len;
  Ppoint3 text_pos;
  Pvec3 plane[2];
  char text[1024];

  Ppoint3 *data;
#ifdef DEBUG
  printf("DEBUG: text3\n");
#endif

  len = FTN_CHARACTER_LEN(chars);
  if (len < 1024) {
    strncpy(&text[0], FTN_CHARACTER_GET(chars), len);
    text[len] = '\0';
    text_pos.x = FTN_REAL_GET(px);
    text_pos.y = FTN_REAL_GET(py);
    text_pos.z = FTN_REAL_GET(pz);
    plane[0].delta_x = FTN_REAL_ARRAY_GET(tdx, 0);
    plane[0].delta_y = FTN_REAL_ARRAY_GET(tdy, 0);
    plane[0].delta_z = FTN_REAL_ARRAY_GET(tdz, 0);
    plane[1].delta_x = FTN_REAL_ARRAY_GET(tdx, 1);
    plane[1].delta_y = FTN_REAL_ARRAY_GET(tdy, 1);
    plane[1].delta_z = FTN_REAL_ARRAY_GET(tdz, 1);

    ptext3(&text_pos, &plane[0], &text[0]);

  } else {
    printf("ERROR: Buffer overlow in PTX3: Ignoring function");
  }
}

