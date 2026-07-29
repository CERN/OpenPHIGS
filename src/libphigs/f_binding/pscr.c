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

extern short int wsgl_use_shaders_settings;
extern int record_geom;

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(popwk)(;
extern FTN_SUBROUTINE(ppost)(;
extern FTN_SUBROUTINE(pscm)(;
extern FTN_SUBROUTINE(pscr)(;
extern FTN_SUBROUTINE(pupost)(;
extern FTN_SUBROUTINE(pupast)(;
extern FTN_SUBROUTINE(prst)(;
extern FTN_SUBROUTINE(pswkw)(;
extern FTN_SUBROUTINE(pswkw3)(;
extern FTN_SUBROUTINE(pclwk)(;
extern FTN_SUBROUTINE(psvwr3)(;
extern FTN_SUBROUTINE(pqhrm)(;
extern FTN_SUBROUTINE(pqvwr)(;
extern FTN_SUBROUTINE(pqwkt)(;
extern FTN_SUBROUTINE(pqwkt3)(;
extern FTN_SUBROUTINE(pqpost)(;
extern FTN_SUBROUTINE(pqwkpo)(;
extern FTN_SUBROUTINE(puwk)(;
extern FTN_SUBROUTINE(pspmr)(;
extern FTN_SUBROUTINE(psplr)(;
extern FTN_SUBROUTINE(psedr)(;
extern FTN_SUBROUTINE(psir)(;
extern FTN_SUBROUTINE(pstxr)(;
extern FTN_SUBROUTINE(pswkv3)(;
extern FTN_SUBROUTINE(psdus)(;
extern FTN_SUBROUTINE(pqcr)(;
extern FTN_SUBROUTINE(pqcf)(;
extern FTN_SUBROUTINE(pmsg)(;
extern FTN_SUBROUTINE(pslsr)(;
extern FTN_SUBROUTINE(pxscm)(;
extern FTN_SUBROUTINE(psfname)(;


/**
 * \file pscr.c
 * \brief Set colour representation
 */

FTN_SUBROUTINE(pscr)(
                     FTN_INTEGER(wkid),
                     FTN_INTEGER(ci),
                     FTN_INTEGER(nccs),
                     FTN_REAL_ARRAY(cspec)
                     )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint ind = FTN_INTEGER_GET(ci);
  Pint ncc = FTN_INTEGER_GET(nccs);
  Pint color_model;
  Pcolr_rep rep;
  Ws *wsh;
  wsh = PHG_WSID(ws_id);
  color_model = wsh->current_colour_model;
#ifdef DEBUG
  printf("DEBUG: PSCR workstation color representation %d\n", *wkid);
#endif
  if (ncc<3 || ncc>4){
    printf("WARNING: PSCR not enough or too many color components %d. Ignoring function.\n", ncc);
  };
  switch (color_model) {
  case PMODEL_RGB:
    rep.rgb.red   = FTN_REAL_ARRAY_GET(cspec, 0);
    rep.rgb.green = FTN_REAL_ARRAY_GET(cspec, 1);
    rep.rgb.blue  = FTN_REAL_ARRAY_GET(cspec, 2);
    break;
  case PMODEL_RGBA:
    rep.rgba.red   = FTN_REAL_ARRAY_GET(cspec, 0);
    rep.rgba.green = FTN_REAL_ARRAY_GET(cspec, 1);
    rep.rgba.blue  = FTN_REAL_ARRAY_GET(cspec, 2);
    if (ncc == 4) {
      rep.rgba.alpha = FTN_REAL_ARRAY_GET(cspec, 3);
    } else {
      printf("INFO: psrc no alpha component specified in RGBA mode. Using 1.\n");
      rep.rgba.alpha = 1,0;
    };
#ifdef DEBUGA
    printf("INFO: psrc set color RGBA %f%f %f %f\n",
           rep.rgba.red,
           rep.rgba.green,
           rep.rgba.blue,
           rep.rgba.alpha
           );
#endif
    break;
  case PINDIRECT:
    rep.rgb.red = rep.rgb.green = rep.rgb.blue = FTN_REAL_ARRAY_GET(cspec, 0);
    break;
  default:
    printf("WARNING: Unknown color model %d. Ignoring function.\n", color_model);
  }
  pset_colr_rep(ws_id, ind, &rep);
}

