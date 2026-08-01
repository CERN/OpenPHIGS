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
 * \file pinst.c
 * \brief initialize string
 */
FTN_SUBROUTINE(pinst)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(stdnr),
                      FTN_INTEGER(lstr),
                      char *istr,
                      FTN_INTEGER(ipet),
                      FTN_REAL(xmin),
                      FTN_REAL(xmax),
                      FTN_REAL(ymin),
                      FTN_REAL(ymax),
                      FTN_INTEGER(ldr),
                      char* datrec){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint string_dev = FTN_INTEGER_GET(stdnr);
  Pint ilen = FTN_INTEGER_GET(lstr);
  Pint pet = FTN_INTEGER_GET(ipet);
  char * init_string = istr;
  Plimit area;
  Pstring_data data;
  int * here;

  char * buffer = (char*)malloc((ilen+1)*sizeof(char));
  if (buffer != NULL) {
    strncpy(buffer, istr, ilen*sizeof(char));
    buffer[ilen] = '\0';
    area.x_min = FTN_REAL_GET(xmin);
    area.x_max = FTN_REAL_GET(xmax);
    area.y_min = FTN_REAL_GET(ymin);
    area.y_max = FTN_REAL_GET(ymax);
    /* decode input data */
    here = (int*)&datrec[0];
    if (here[0] == 2){
      data.buffer_size = here[1];
      data.init_pos = here[2];

      /* only one echo mode */
      data.pets.pet_r1.unused = 0;
      pinit_string(ws_id, string_dev, buffer, pet, &area, &data);
    } else {
      printf("ERROR in pinst: wrong number of integers in data record\n");
    }
    free(buffer);
  }
}

