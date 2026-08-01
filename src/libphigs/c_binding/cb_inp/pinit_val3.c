/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
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

#include "phg.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "private/sinqP.h"
#include "private/wsxP.h"

/**
 * \file pinit_val3.c
 * \brief Initialize valuator3
 */
void pinit_val3(
                Pint ws_id,
                Pint val_dev,
                Pfloat init_value,
                Pint pet,
                Plimit3 *echo_volume,
                Pval_data *val_data_rec
                )
{
  Wst_input_wsdt *idt;
  Wst_phigs_dt *dt;
  Phg_args_inp_init_dev args;
  Ws_handle wsh;
  Plimit3 echo_vol;
  Wst_defval *ddt;
  Pint i, string_len=80;

  /* TODO: Change to only accept outin workstations */
  idt = input_ws_open(ws_id, Pfn_init_val3, &dt, NULL);
  if (idt != NULL) {
#ifdef DEBUGINP
    printf("pinit_val3 called for device %d\n", val_dev);
#endif
    if ((val_dev > 0) && (val_dev <= idt->num_devs.val)) {
      if (phg_echo_limits_valid(ws_id, Pfn_init_val3, echo_volume, dt)) {
        ddt = &idt->valuators[val_dev - 1];
        if (!phg_int_in_list(pet, ddt->num_pets, ddt->pets)) {
          /* Report error and use default data */
          ERR_REPORT(PHG_ERH, ERR253);
          pet = 0;
        }
        args.wsid                   = ws_id;
        args.idev_class             = PHG_ARGS_INP_VAL3;
        args.dev                    = val_dev;
        args.pet                    = pet;
        args.data.val.init          = init_value;
        /* String length including the terminating zero */
        if (pet<0) {
          if (val_data_rec->pets.pet_u1.label != NULL){
            args.data.val.counts[0] = 1 + strlen(val_data_rec->pets.pet_u1.label);
          } else {
            args.data.val.counts[0] = 0;
          }
          if (val_data_rec->pets.pet_u1.format != NULL){
            args.data.val.counts[1] = 1 + strlen(val_data_rec->pets.pet_u1.format);
          } else {
            args.data.val.counts[1] = 0;
          }
          if (val_data_rec->pets.pet_u1.low_label != NULL){
            args.data.val.counts[2] = 1 + strlen(val_data_rec->pets.pet_u1.low_label);
          } else {
            args.data.val.counts[2] = 0;
          }
          if (val_data_rec->pets.pet_u1.high_label != NULL){
            args.data.val.counts[3] = 1 + strlen(val_data_rec->pets.pet_u1.high_label);
          } else {
            args.data.val.counts[3] = 0;
          }
        }
        memcpy(&args.echo_volume,
               echo_volume,
               sizeof(Plimit3));
        memcpy(&args.data.val.rec,
               val_data_rec,
               sizeof(Pval_data3));
        wsh = PHG_WSID(ws_id);
        (*wsh->init_device)(wsh, &args);
      }
      else {
        ERR_REPORT(PHG_ERH, ERR260);
      }
      /* Error reported by phg_echo_limits_valid */
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

