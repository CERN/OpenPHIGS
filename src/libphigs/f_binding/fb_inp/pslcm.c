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
#include <math.h>
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <private/wsxP.h>
#include <util/ftn.h>

/**
 * \file pslcm.c
 * \brief set locator mode
 */
FTN_SUBROUTINE(pslcm)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(lcdnr),
                      FTN_INTEGER(mode),
                      FTN_INTEGER(echo)
                      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint dev_id = FTN_INTEGER_GET(lcdnr);
  Pint mode1 = FTN_INTEGER_GET(mode);
  Pint echo1 = FTN_INTEGER_GET(echo);
  pset_loc_mode(ws_id, dev_id, mode1, echo1);
}

