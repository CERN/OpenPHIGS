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
 * \file pshrm.c
 *
 * \brief       set the hlhsr mode for the workstation
 * \param       wkid workstation ID
 * \param       hrm mode
 * \verbatim
 Possible values:
   0  HLHRS off
   1  Use Z-Buffer
 * \endverbatim
 * \sa  pqhrm pshrid
 */
FTN_SUBROUTINE(pshrm)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(hrm)
                      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint mode = FTN_INTEGER_GET(hrm);
#ifdef DEBUG
  printf("DEBUG: PSHRM set hlrs mode to %d.\n", mode);
#endif
  pset_hlhsr_mode(ws_id, mode);
}

