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
 * \file pinlc3.c
 * \brief initialize locator 3
 */
FTN_SUBROUTINE(pinlc3)(
                       FTN_INTEGER(wkid),
                       FTN_INTEGER(lcdnr),
                       FTN_INTEGER(iviewi),
                       FTN_REAL(ipx),
                       FTN_REAL(ipy),
                       FTN_REAL(ipz),
                       FTN_INTEGER(pet1),
                       FTN_REAL_ARRAY(evol),
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
  printf("DEBUG: PINLC3 setup 3d locator\n");
#endif

  location.x = FTN_REAL_GET(ipx);
  location.y = FTN_REAL_GET(ipy);
  location.z = FTN_REAL_GET(ipz);
  lim.x_min = FTN_REAL_ARRAY_GET(evol, 0);
  lim.x_max = FTN_REAL_ARRAY_GET(evol, 1);
  lim.y_min = FTN_REAL_ARRAY_GET(evol, 2);
  lim.y_max = FTN_REAL_ARRAY_GET(evol, 3);
  lim.z_min = FTN_REAL_ARRAY_GET(evol, 4);
  lim.z_max = FTN_REAL_ARRAY_GET(evol, 5);

  /* FIXME here we may need some more stuff */
  data.pets.pet_r1.unused = 0;
  /* FIXME check if the data record is actually being used */
  pinit_loc3(ws_id, loc_num, init_view_ind, &location, pet, &lim, &data);

};

