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
*******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "util/ftn.h"
#include "phconf.h"

/*******************************************************************************
 * pxndef
 *
 * DESCR:       Read configuration from file
 * RETURNS:     N/A
 */
#define MAXLEN 4096
FTN_SUBROUTINE(pxndef)(
		       FTN_CHARACTER(NAME)
		       )
{
  char name[MAXLEN+1];
  char * filename = FTN_CHARACTER_GET(NAME);
  Pint length = FTN_CHARACTER_LEN(NAME);
  if (length <= MAXLEN) {
    strncpy(name, filename, length);
    name[length] = '\0';
    read_config(name);
  } else {
    printf("WARNING: Configuration file name is too long\n");
  }
}
