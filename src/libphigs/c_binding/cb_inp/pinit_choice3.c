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
 * \file pinit_choice3.c
 * \brief Initialize choice3
 */
void pinit_choice3(
                   Pint ws_id,
                   Pint choice_dev,
                   Pin_status init_status,
                   Pint init_choice,
                   Pint pet,
                   Plimit3 * echo_volume,
                   Pchoice_data3 *choice_data_rec
                   )
{
  Wst_input_wsdt *idt;
  Wst_phigs_dt *dt;
  Phg_args_inp_init_dev args;
  Ws_handle wsh;
  Plimit3 echo_vol;
  Wst_defchoice *ddt;

   /* TODO: Change to only accept outin workstations */
  idt = input_ws_open(ws_id, Pfn_init_choice3, &dt, NULL);
  if (idt != NULL) {
    if ((choice_dev > 0) && (choice_dev <= idt->num_devs.choice)) {
      if (phg_echo_limits_valid(ws_id, Pfn_init_choice3, echo_volume, dt)) {
        ddt = &idt->choices[choice_dev - 1];
        if (!phg_int_in_list(pet, ddt->num_pets, ddt->pets)) {
          /* Report error and use default data */
          ERR_REPORT(PHG_ERH, ERR253);
          pet = 1;
        }
        args.wsid                   = ws_id;
        args.idev_class             = PHG_ARGS_INP_CHC3;
        args.dev                    = choice_dev;
        args.pet                    = pet;
        args.data.cho.init = init_choice;
        args.data.cho.status = init_status;
        memcpy(&args.echo_volume,
               echo_volume,
               sizeof(Plimit3));
        memcpy(&args.data.cho.rec,
               choice_data_rec,
               sizeof(Pchoice_data3));
        /*
          pet1 : unused
          pet2 : num_prompts and list of Ppr_switch in *prompts
          pet3 : num strings and list of strings
          pet4 : as pet3
          pet5 : struct_id, num_pick_ids list of Pints in pick_ids (?)
        */
        switch (pet) {
        case 1:
        case -1:
          args.data.cho.string_list_size = 0;
          args.data.cho.rec.pets.pet_r1.unused = 0;
          break;
        case 3:
        case -3:
          args.data.cho.string_list_size = choice_data_rec->pets.pet_r3.num_strings;
          args.data.cho.rec.pets.pet_r3.num_strings = choice_data_rec->pets.pet_r3.num_strings;
          args.data.cho.rec.pets.pet_r3.strings = choice_data_rec->pets.pet_r3.strings;
          break;
        case 4:
        case -4:
          args.data.cho.string_list_size = choice_data_rec->pets.pet_r4.num_strings;
          args.data.cho.rec.pets.pet_r4.num_strings = choice_data_rec->pets.pet_r4.num_strings;
          args.data.cho.rec.pets.pet_r4.strings = choice_data_rec->pets.pet_r4.strings;
          break;
        }
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

