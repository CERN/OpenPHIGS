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
#include "util/ftn.h"

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(ptx)(;
extern FTN_SUBROUTINE(ptx3)(;
extern FTN_SUBROUTINE(pstxfn)(;
extern FTN_SUBROUTINE(pstxpr)(;
extern FTN_SUBROUTINE(pstxp)(;
extern FTN_SUBROUTINE(pstxal)(;
extern FTN_SUBROUTINE(pstxci)(;
extern FTN_SUBROUTINE(pstxi)(;
extern FTN_SUBROUTINE(patr)(;
extern FTN_SUBROUTINE(patr3)(;
extern FTN_SUBROUTINE(psatch)(;
extern FTN_SUBROUTINE(psatal)(;
extern FTN_SUBROUTINE(psatp)(;


/**
 * \file patr.c
 * \brief Annotation text relative
 */
FTN_SUBROUTINE(patr)(
                     FTN_REAL(rpx),
                     FTN_REAL(rpy),
                     FTN_REAL(apx),
                     FTN_REAL(apy),
                     FTN_CHARACTER(chars)
                     ){
  Ppoint ref_point;
  Pvec offset;
  int len = FTN_CHARACTER_LEN(chars);
  char * text = (char*)malloc(len+1);
  memcpy(text, FTN_CHARACTER_GET(chars), len);
  text[len] = '\0';
  ref_point.x = FTN_REAL_GET(rpx);
  ref_point.y = FTN_REAL_GET(rpy);
  offset.delta_x = FTN_REAL_GET(apx);
  offset.delta_y = FTN_REAL_GET(apy);
  panno_text_rel(&ref_point, &offset, text);
}

