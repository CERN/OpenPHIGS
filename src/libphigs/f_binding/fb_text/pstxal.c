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
 * \file pstxal.c
 * \brief Set text alignment
 */
FTN_SUBROUTINE(pstxal)(
                       FTN_INTEGER(txalh),
                       FTN_INTEGER(txalv)
                       )
{
#ifdef DEBUG
  printf("DEBUG: PSTXAL text align called\n");
#endif
  Ptext_align text_align;
  text_align.hor = (Phor_text_align) FTN_INTEGER_GET(txalh);
  text_align.vert = (Pvert_text_align) FTN_INTEGER_GET(txalv);
  pset_text_align(&text_align);
}

