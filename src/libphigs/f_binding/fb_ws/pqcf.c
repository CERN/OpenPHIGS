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
 * \file pqcf.c
 * \brief Inquire colour facilities
 *
 * \return error index, number of colour indices, colour available, number of predefined colour indices, primary colours
 */

FTN_SUBROUTINE(pqcf)(
                     FTN_INTEGER(wtype),
                     int* errind,
                     int* ncoli,
                     int* cola,
                     int* npci,
                     float* cc) {
  printf("WARNING: pqcf called for WSTYPE: %d. Returning DUMMY values\n", FTN_INTEGER_GET(wtype));
  *errind = 0;
  *ncoli = 15;
  *cola = 1;
  cc[0]=1.;
  cc[1]=0.;
  cc[2]=0.;
  cc[3]=0.;
  cc[4]=1.;
  cc[5]=0.;
  cc[6]=0.;
  cc[7]=0.;
  cc[8]=1.;
}

