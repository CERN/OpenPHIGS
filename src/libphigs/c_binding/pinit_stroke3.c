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
 * \file pinit_stroke3.c
 * \brief Initialize stroke device 3D
 */
void pinit_stroke3(
                   Pint ws_id,
                   Pint stroke_num,
                   Pint init_view_ind,
                   Ppoint_list3 *init_stroke,
                   Pint pet,
                   Plimit3 *echo_vol,
                   Pstroke_data3 *stroke_data
                   )
{
  Wst_input_wsdt *idt;
  Wst_phigs_dt *dt;
  Wst_defstroke *ddt;
  Phg_args_inp_init_dev args;
  Ws_handle wsh;

  idt = input_ws_open(ws_id, Pfn_init_stroke3, &dt, NULL);
  if (idt != NULL) {
    if ((stroke_num > 0) &&  (stroke_num <= idt->num_devs.stroke)) {
      if (phg_echo_limits_valid(ws_id, Pfn_init_stroke3, echo_vol, dt)) {
        /* TODO: Also check against maximum view index */
        if (init_view_ind >= 0) {
          ddt = &idt->strokes[stroke_num - 1];
          if (!phg_int_in_list(pet, ddt->num_pets, ddt->pets)) {
            /* Report error and use default data */
            ERR_REPORT(PHG_ERH, ERR253);
            stroke_data = &ddt->record;
            pet = 1;
          }
          if (check_stroke_data_record(pet, stroke_data, dt, ddt)) {
            if (init_stroke->num_points < stroke_data->buffer_size) {
              args.wsid                     = ws_id;
              args.idev_class               = PHG_ARGS_INP_STK3;
              args.dev                      = stroke_num;
              args.pet                      = pet;
              args.data.stk.init.view_ind   = init_view_ind;
              args.data.stk.init.num_points = init_stroke->num_points;
              args.data.stk.init.points     = init_stroke->points;
              memcpy(&args.echo_volume,
                     echo_vol,
                     sizeof(Plimit3));
              memcpy(&args.data.stk.rec,
                     stroke_data,
                     sizeof(Pstroke_data3));
              wsh = PHG_WSID(ws_id);
              (*wsh->init_device)(wsh, &args);
            }
            else {
              ERR_REPORT(PHG_ERH, ERR262);
            }
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

