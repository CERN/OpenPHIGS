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
 * \file pspmr.c
 * \brief Set polymarker representation
 */
FTN_SUBROUTINE(pspmr)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(pmi),
                      FTN_INTEGER(mtype),
                      FTN_REAL(mszsf),
                      FTN_INTEGER(coli)
                      ){
  Pint wk_id = FTN_INTEGER_GET(wkid);
  Pint pind  = FTN_INTEGER_GET(pmi);
  Pint ptype = FTN_INTEGER_GET(mtype);
  Pfloat size  = FTN_REAL_GET(mszsf);
  Pint col = FTN_INTEGER_GET(coli);
  Pmarker_bundle mkrep = { ptype, size, col };
  pset_marker_rep(wk_id, pind, &mkrep);
}

