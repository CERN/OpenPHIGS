/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
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
******************************************************************************
* Changes:   Copyright (C) 2022-2023 CERN
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "util/ftn.h"

/**
 * \file pshrid.c
 *
 * \brief       hlhrs mode identifier, hidden line mode
 * \param       hrid set hidden line mode
 * \verbatim
 Possible values:

  0 PHIGS_HLHSR_ID_OFF Hidden line off
  1 PHIGS_HLHSR_ID_ON  Hidden line on
  2 PHIGS_HLHSR_ID_ON_NZ Hidden line on
 * \endverbatim
 * \note Adds a new element to the current structure
 * \pre The current structure must be open for editing
 * \sa pshrm pqhrm
 */
FTN_SUBROUTINE(pshrid)(
                       FTN_INTEGER(hrid)
                       )
{
  Pint hrmode = FTN_INTEGER_GET(hrid);
#ifdef DEBUG
  printf("DEBUG: PSHRID set hlrs id to %d.\n", hrmode);
#endif
  switch(hrmode){
  case 0:
    pset_hlhsr_id(PHIGS_HLHSR_ID_OFF);
    break;
  case 1:
    pset_hlhsr_id(PHIGS_HLHSR_ID_ON);
    break;
  case 2:
    pset_hlhsr_id(PHIGS_HLHSR_ID_ON_NZ);
    break;
  }
}

