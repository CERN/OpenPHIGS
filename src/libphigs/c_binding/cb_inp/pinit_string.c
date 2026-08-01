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
 * \file pinit_string.c
 * \brief Initialize string
 */
void pinit_string(
                  Pint ws_id,
                  Pint string_dev,
                  char * init_string,
                  Pint pet,
                  Plimit * area,
                  Pstring_data *string_data
                  )
{
  Wst_input_wsdt *idt;
  Wst_phigs_dt *dt;
  Wst_defstring *ddt;
  Phg_args_inp_init_dev args;
  Ws_handle wsh;
  Plimit3 echo_vol;

  echo_vol.x_min = area->x_min;
  echo_vol.x_max = area->x_max;
  echo_vol.y_min = area->y_min;
  echo_vol.y_max = area->y_max;
  echo_vol.z_min = 0.0;
  echo_vol.z_max = 0.0;

  /* TODO: Change to only accept outin workstations */
  idt = input_ws_open(ws_id, Pfn_init_string, &dt, NULL);
  if (idt != NULL) {
    if ((string_dev > 0) && (string_dev <= idt->num_devs.string)) {
      if (phg_echo_limits_valid(ws_id, Pfn_init_string, &echo_vol, dt)) {
        ddt = &idt->strings[string_dev - 1];
        if (!phg_int_in_list(pet, ddt->num_pets, ddt->pets)) {
          /* Report error and use default data */
          ERR_REPORT(PHG_ERH, ERR253);
          string_data = &ddt->record;
          pet = 0;
        }
        args.wsid                   = ws_id;
        args.idev_class             = PHG_ARGS_INP_STR;
        args.dev                    = string_dev;
        args.pet                    = pet;
        args.data.str.init.length   = strlen(init_string);
        args.data.str.init.string   = init_string;
        memcpy(&args.echo_volume,
               &echo_vol,
               sizeof(Plimit3));
        memcpy(&args.data.str.rec,
               string_data,
               sizeof(Pstring_data));
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

