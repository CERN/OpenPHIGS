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
 * \file ppost.c
 * \brief Post structure
 */

FTN_SUBROUTINE(ppost)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(strid),
                      FTN_REAL(priort)
                      )
{
  int status;
  Ws_handle wsh;
  Css_handle cssh;
  Struct_handle structp;

#ifdef DEBUG
  printf("DEBUG: PPOST structure to %d\n", *wkid);
#endif

  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint struct_id = FTN_INTEGER_GET(strid);
  Pfloat priority = FTN_REAL_GET()priort;

  if (phg_ws_open(ws_id, Pfn_post_struct) != NULL) {
    wsh = PHG_WSID(ws_id);
    cssh = wsh->out_ws.model.b.cssh;
    structp = phg_css_post(cssh, struct_id, wsh, &status);
    if (structp != NULL) {
      (*wsh->post)(wsh, structp, priority, !status);
    }
  }
}

