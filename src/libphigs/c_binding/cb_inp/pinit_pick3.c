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
 * \file pinit_pick3.c
 * \brief Initialize pick device 3D
 */
void pinit_pick3(
                 Pint ws_id,
                 Pint pick_num,
                 Pin_status init_status,
                 Ppick_path *init_pick,
                 Pint pet,
                 Plimit3 *echo_vol,
                 Ppick_data3 *pick_data,
                 Ppath_order order
                 )
{
  Wst_input_wsdt *idt;
  Wst_phigs_dt *dt;
  Wst_defpick *ddt;
  Phg_args_inp_init_dev args;
  Ws_handle wsh;

  /* TODO: Change to only accept outin workstations */
  idt = input_ws_open(ws_id, Pfn_init_pick3, &dt, NULL);
  if (idt != NULL) {
    if ((pick_num > 0) &&  (pick_num <= idt->num_devs.pick)) {
      if (phg_echo_limits_valid(ws_id, Pfn_init_pick3, echo_vol, dt)) {
        ddt = &idt->picks[pick_num - 1];
        if (!phg_int_in_list(pet, ddt->num_pets, ddt->pets)) {
          /* Report error and use default data */
          ERR_REPORT(PHG_ERH, ERR253);
          pick_data = &ddt->record;
          pet = 1;
        }
        if (check_pick_data_record(pet, pick_data, dt, ddt)) {
          args.wsid                   = ws_id;
          args.idev_class             = PHG_ARGS_INP_PIK3;
          args.dev                    = pick_num;
          args.pet                    = pet;
          args.data.pik.init.status   = init_status;
          if (init_status == PIN_STATUS_OK) {
            memcpy(&args.data.pik.init.pick_path,
                   init_pick,
                   sizeof(Ppick_path));
          }
          else {
            args.data.pik.init.pick_path.depth = 0;
          }
          memcpy(&args.echo_volume,
                 echo_vol,
                 sizeof(Plimit3));
          memcpy(&args.data.pik.rec,
                 pick_data,
                 sizeof(Ppick_data3));
          args.data.pik.porder = order;
          wsh = PHG_WSID(ws_id);
          (*wsh->init_device)(wsh, &args);
        }
        else {
          ERR_REPORT(PHG_ERH, ERR260);
        }
      }
      /* Error reported by phg_echo_limits_valid */
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

