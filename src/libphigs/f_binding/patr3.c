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
 * \file patr3.c
 * \brief ANNOTATION TEXT RELATIVE 3
 */
FTN_SUBROUTINE(patr3)(
                      FTN_REAL(rpx),
                      FTN_REAL(rpy),
                      FTN_REAL(rpz),
                      FTN_REAL(apx),
                      FTN_REAL(apy),
                      FTN_REAL(apz),
                      FTN_CHARACTER(chars)
                      ){
  Ppoint3 ref_point;
  Pvec3 offset;
  int len = FTN_CHARACTER_LEN(chars);
  char * text = (char*)malloc(len+1);
  memcpy(text, FTN_CHARACTER_GET(chars), len);
  text[len] = '\0';

  ref_point.x = FTN_REAL_GET(rpx);
  ref_point.y = FTN_REAL_GET(rpy);
  ref_point.z = FTN_REAL_GET(rpz);
  offset.delta_x = FTN_REAL_GET(apx);
  offset.delta_y = FTN_REAL_GET(apy);
  offset.delta_z = FTN_REAL_GET(apz);
  panno_text_rel3(&ref_point, &offset, text);
}

