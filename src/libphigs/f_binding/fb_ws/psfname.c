/******************************************************************************
 *   Do NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gl2ps.h>
#include "phigs.h"
#include "phg.h"
#include "private/phgP.h"
#include "private/wsglP.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"

extern int record_geom;
/**
 * \file psfname.c
 *
 * \brief       Set workstation output file name for hardcopy workstation types
 * \note        This function is an extension specific to OpenPHIGS
 *
 * \sa popwk
 */
FTN_SUBROUTINE(psfname)(
                        FTN_INTEGER(wkid),
                        FTN_CHARACTER(fname)
                        )
{
  Ws_handle wsh;
  Pint ws_id = FTN_INTEGER_GET(wkid);
  char * filename = FTN_CHARACTER_GET(NAME);
  int length = FTN_CHARACTER_LEN(fname);
  wsh = PHG_WSID(ws_id);
  if (wsh != NULL){
    strncpy(wsh->filename, filename, length);
    (wsh->filename)[length] = '\0';
  } else {
    printf("ERROR in PSFNAME: workstation %d not found.", ws_id);
  }
}
