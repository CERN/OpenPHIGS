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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "phigs.h"
#include "phg.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"

/**
 * \file pqdsp3.c
 * \brief Inquire display space size 3
 *
 * \return error index, coordinate units, max display volume size (DC) max display volume size in raster units
 */
FTN_SUBROUTINE(pqdsp3)(
                       FTN_INTEGER(wstype),
                       Pint* err_ind,
                       Pint * dcunit,
                       Pfloat * dx,
                       Pfloat * dy,
                       Pfloat * dz,
                       Pint * rx,
                       Pint * ry,
                       Pint * rz
                       ){
  Pint ws_type = FTN_INTEGER_GET(wstype);
  Pdisp_space_size3 size;
  pinq_disp_space_size3(ws_type, err_ind, &size);
  if (err_ind == 0) {
    *dcunit = (Pint) size.dc_units;
    *dx = size.size_dc.size_x;
    *dy = size.size_dc.size_y;
    *dz = size.size_dc.size_z;
    *rx = size.size_raster.size_x;
    *ry = size.size_raster.size_y;
    *rz = size.size_raster.size_z;
  }
}
