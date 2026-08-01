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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <private/wsxP.h>
#include <util/ftn.h>

/**
 * \file pinlc.c
 * \brief initialize locator
 */
FTN_SUBROUTINE(pinlc)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(lcdnr),
                      FTN_INTEGER(iviewi),
                      FTN_REAL(ipx),
                      FTN_REAL(ipy),
                      FTN_INTEGER(pet1),
                      FTN_REAL(xmin),
                      FTN_REAL(xmax),
                      FTN_REAL(ymin),
                      FTN_REAL(ymax),
                      FTN_INTEGER(ldr),
                      char* datrec
                      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint loc_num = FTN_INTEGER_GET(lcdnr);
  Pint init_view_ind = FTN_INTEGER_GET(iviewi);
  Pint pet = FTN_INTEGER_GET(pet1);
  Ppoint3 location;
  Plimit3 lim;
  Ploc_data3 data;

#ifdef DEBUG
  printf("DEBUG: PSCHSP setup 3d locator\n");
#endif

  location.x = FTN_REAL_GET(ipx);
  location.y = FTN_REAL_GET(ipy);
  location.z = 0.0;
  lim.x_min = FTN_REAL_GET(xmin);
  lim.x_max = FTN_REAL_GET(xmax);
  lim.y_min = FTN_REAL_GET(ymin);
  lim.y_max = FTN_REAL_GET(ymax);
  lim.z_min = 0.0;
  lim.z_max = 0.0;

  /* only echo mode 1 used */
  data.pets.pet_r1.unused = 0;
  /* FIXME check if the data is being used */
  pinit_loc3(ws_id, loc_num, init_view_ind, &location, pet, &lim, &data);
};

