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
 * \file pinit_loc3.c
 * \brief Initialize locator device 3D
 */
void pinit_loc3(
                Pint ws_id,
                Pint loc_num,
                Pint init_view_ind,
                Ppoint3 *init_loc_pos,
                Pint pet,
                Plimit3 *echo_vol,
                Ploc_data3 *loc_data
                )
{
  Wst_input_wsdt *idt;
  Wst_phigs_dt *dt;
  Wst_defloc *ddt;
  Phg_args_inp_init_dev args;
  Ws_handle wsh;

  idt = input_ws_open(ws_id, Pfn_init_loc3, &dt, NULL);
  if (idt != NULL) {
    if ((loc_num > 0) &&  (loc_num <= idt->num_devs.loc)) {
      if (phg_echo_limits_valid(ws_id, Pfn_init_loc3, echo_vol, dt)) {
        /* TODO: Also check against maximum view index */
        if (init_view_ind >= 0) {
          ddt = &idt->locators[loc_num - 1];
          if (!phg_int_in_list(pet, ddt->num_pets, ddt->pets)) {
            /* Report error and use default data */
            ERR_REPORT(PHG_ERH, ERR253);
            loc_data = &ddt->record;
            pet = 1;
          }
          if (check_loc_data_record(pet, loc_data, dt, ddt)) {
            args.wsid                   = ws_id;
            args.idev_class             = PHG_ARGS_INP_LOC3;
            args.dev                    = loc_num;
            args.pet                    = pet;
            args.data.loc.init.view_ind = init_view_ind;
            memcpy(&args.data.loc.init.position,
                   init_loc_pos,
                   sizeof(Ppoint3));
            memcpy(&args.echo_volume,
                   echo_vol,
                   sizeof(Plimit3));
            memcpy(&args.data.loc.rec,
                   loc_data,
                   sizeof(Ploc_data3));
            wsh = PHG_WSID(ws_id);
            (*wsh->init_device)(wsh, &args);
               }
          else {
            ERR_REPORT(PHG_ERH, ERR260);
          }
        }
        else {
          ERR_REPORT(PHG_ERH, ERR114);
        }
      }
      /* Error reported by phg_echo_limits_valid */
    }
    else {
      ERR_REPORT(PHG_ERH, ERR250);
    }
  }
}

