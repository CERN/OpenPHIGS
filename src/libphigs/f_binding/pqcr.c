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

extern short int wsgl_use_shaders_settings;
extern int record_geom;

/**
 * \file pqcr.c
 * \brief Inquire colour representation
 *
 * \return Error index, number of colour components, colour specs
 */

FTN_SUBROUTINE(pqcr)(
                     FTN_INTEGER(wkid),
                     FTN_INTEGER(coli),
                     FTN_INTEGER(ccsbsz),
                     FTN_INTEGER(rtype),
                     int* err_ind,
                     int* ol,
                     float* cspec
                     ){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint colr_ind = FTN_INTEGER_GET(coli);
  Pinq_type type = (Pinq_type) FTN_INTEGER_GET(rtype);
  Pint buf_size = FTN_INTEGER_GET(ccsbsz);
  Pcolr_rep colr_rep;
  if (buf_size >= 3){
    pinq_colr_rep(ws_id, colr_ind, type, err_ind, &colr_rep);
    if (*err_ind == 0){
      switch (buf_size) {
      case 3:
        cspec[0] = colr_rep.rgb.red;
        cspec[1] = colr_rep.rgb.green;
        cspec[2] = colr_rep.rgb.blue;
        break;
      case 4:
        *ol = 4;
        cspec[0] = colr_rep.rgba.red;
        cspec[1] = colr_rep.rgba.green;
        cspec[2] = colr_rep.rgba.blue;
        cspec[3] = colr_rep.rgba.alpha;
        break;
      default:
        *ol = 0;
        cspec[0] = 0;
        cspec[1] = 0;
        cspec[2] = 0;
        cspec[3] = 1;
        printf("Error in pqcr: Given buffer is too small\n");
        break;
      }
    }
  } else {
    *err_ind = 1;
    printf("pqcr: Buffer size too small. Ignoring function.\n");
  }
}

