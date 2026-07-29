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
 * \file pinvl3.c
 * \brief initialize valuators 3
 */
FTN_SUBROUTINE(pinvl3)(
                       FTN_INTEGER(wkid),
                       FTN_INTEGER(vldnr),
                       FTN_REAL(ival),
                       FTN_INTEGER(ipet),
                       FTN_REAL_ARRAY(evol),
                       FTN_INTEGER(ldr),
                       char* datrec) {
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint val_dev = FTN_INTEGER_GET(vldnr);
  Pfloat init_value =FTN_REAL_GET(ival);
  Pint pet = FTN_INTEGER_GET(ipet);
  Plimit3 echo_volume;
  Pval_data val_data_rec;
  Pfloat * rp;
  Pint * ip;
  char* cp;
  char* buffer;
  int i, length, l1, l2, l3, l4;
  int nstrings, charlen;
  Pint num_boxed;
  echo_volume.x_min = FTN_REAL_ARRAY_GET(evol, 0);
  echo_volume.x_max = FTN_REAL_ARRAY_GET(evol, 1);
  echo_volume.y_min = FTN_REAL_ARRAY_GET(evol, 2);
  echo_volume.y_max = FTN_REAL_ARRAY_GET(evol, 3);
  echo_volume.z_min = FTN_REAL_ARRAY_GET(evol, 4);
  echo_volume.z_max = FTN_REAL_ARRAY_GET(evol, 5);
  ip = (int*)&datrec[0]; //  number of ints;
  switch (abs(pet)){
  case 3:
    if (ip[0] != 1) printf("WARNING: Wrong number of integers for echo mode. Expected 1 but found %d\n", ip[0]);
    num_boxed = ip[1];
    ip = &ip[1];
    break;
  default:
    if (ip[0] != 0) printf("WARNING: Wrong number of integers. Expected 0 but found %d\n", ip[0]);
    num_boxed = 0;
    break;
  }
  /* How many valuator devices to be put into a single box */
  if (ip[1] != 2) printf("WARNING: Wrong number of floats. Expected 2 but found %d\n", ip[1]);
  rp = (float*)&ip[2];
  val_data_rec.low = rp[0];
  val_data_rec.high = rp[1];
  val_data_rec.num_boxed = num_boxed;
  val_data_rec.pets.pet_u1.label = NULL;
  val_data_rec.pets.pet_u1.format = NULL;
  val_data_rec.pets.pet_u1.low_label = NULL;
  val_data_rec.pets.pet_u1.high_label = NULL;
  ip = (int*)&rp[2];
  if (pet <0){
    nstrings = ip[0];
    switch (nstrings){
    case 1:
      l1 = ip[1] + 1;
      l2 = 0;
      l3 = 0;
      l4 = 0;
      break;
    case 2:
      l1 = ip[1] + 1;
      l2 = ip[2] + 1;
      l3 = 0;
      l4 = 0;
      break;
    case 3:
      l1 = ip[1] + 1;
      l2 = ip[2] + 1;
      l3 = ip[3] + 1;
      l4 = 0;
    case 4:
      l1 = ip[1] + 1;
      l2 = ip[2] + 1;
      l3 = ip[3] + 1;
      l4 = ip[4] + 1;
    }
    /* FIXME: release space afterwards */
    cp = (char *)&ip[nstrings+1];
    if (l1 > 0){
      buffer = (char*) malloc((l1+1)*sizeof(char));
      if (buffer != NULL){
        strncpy(buffer, &cp[0], l1*sizeof(char));
        buffer[l1] = '\0';
        val_data_rec.pets.pet_u1.label = buffer;
      } else {
        val_data_rec.pets.pet_u1.label = WST_DEFAULT_VALUATOR_LABEL;
      }
      cp += l1*sizeof(char);
    } else {
      val_data_rec.pets.pet_u1.label = WST_DEFAULT_VALUATOR_LABEL;
    }
    if (l2 > 0){
      buffer = (char*) malloc((l2+1)*sizeof(char));
      if (buffer != NULL){
        strncpy(buffer, &cp[0], l2*sizeof(char));
        buffer[l2] = '\0';
        val_data_rec.pets.pet_u1.format = buffer;
      } else {
        val_data_rec.pets.pet_u1.format = WST_DEFAULT_VALUATOR_FORMAT;
      }
      cp += l2*sizeof(char);
    } else {
      val_data_rec.pets.pet_u1.format = WST_DEFAULT_VALUATOR_FORMAT;
    }
    if (l3 > 0){
      buffer = (char*) malloc((l3+1)*sizeof(char));
      if (buffer != NULL){
        strncpy(buffer, &cp[0], l3*sizeof(char));
        buffer[l3] = '\0';
        val_data_rec.pets.pet_u1.low_label = buffer;
      } else {
        val_data_rec.pets.pet_u1.low_label = WST_DEFAULT_VALUATOR_LOW_LABEL;
      }
      cp += l3*sizeof(char);
    } else {
      val_data_rec.pets.pet_u1.low_label = WST_DEFAULT_VALUATOR_LOW_LABEL;
    }
    if (l4 > 0){
      buffer = (char*) malloc((l4+1)*sizeof(char));
      if (buffer != NULL){
        strncpy(buffer, &cp[0], l4*sizeof(char));
        buffer[l4] = '\0';
        val_data_rec.pets.pet_u1.high_label = buffer;
      } else {
        val_data_rec.pets.pet_u1.high_label = WST_DEFAULT_VALUATOR_HIGH_LABEL;
      }
      cp += l4*sizeof(char);
    } else {
      val_data_rec.pets.pet_u1.high_label = WST_DEFAULT_VALUATOR_HIGH_LABEL;
    }
  }
  pinit_val3(ws_id, val_dev, init_value, pet, &echo_volume, &val_data_rec);
}

