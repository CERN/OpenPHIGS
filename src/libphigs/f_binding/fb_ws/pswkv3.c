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
#include "private/cb_internal.h"
#include "private/wsglP.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"

extern int record_geom;

/**
 * \file pswkv3.c
 * \brief Set workstation viewport 3
 */

FTN_SUBROUTINE(pswkv3)(
                       FTN_INTEGER(wkid),
                       FTN_REAL_ARRAY(wkvp)
                       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Plimit3 lim3;
  lim3.x_min = FTN_REAL_ARRAY_GET(wkvp, 0);
  lim3.x_max = FTN_REAL_ARRAY_GET(wkvp, 1);
  lim3.y_min = FTN_REAL_ARRAY_GET(wkvp, 2);
  lim3.y_max = FTN_REAL_ARRAY_GET(wkvp, 3);
  lim3.z_min = FTN_REAL_ARRAY_GET(wkvp, 4);
  lim3.z_max = FTN_REAL_ARRAY_GET(wkvp, 5);
  pset_ws_vp3(ws_id, &lim3);
}

