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

/* Automatically generated prototypes */
extern #define MAXLEN 4096
FTN_SUBROUTINE(pxndef)(;
extern FTN_SUBROUTINE(pxshcsf)(;
extern FTN_SUBROUTINE(pxqhcsf)(;


/**
 * \file pxshcsf.c
 * \brief set the hardcopy scale factor for workstation
 */
FTN_SUBROUTINE(pxshcsf)(
                        FTN_INTEGER(wkid),
                        FTN_REAL(hcsf)
                       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pfloat hc_sf = FTN_REAL_GET(hcsf);
  pxset_conf_hcsf(ws_id, hc_sf);
}

