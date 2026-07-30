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
#include "util.h"

/**
 * \file pawait_event.c
 * \brief Wait for event to occur
 */
void pawait_event(
                  Pfloat timeout,
                  Pint *ws_id,
                  Pin_class *dev_class,
                  Pint *in_num
                  )
{
  Phg_ret ret;
  unsigned size;
  Ppoint3 *pts;
  Ppick_path_elem *path;
  time_t time, time1, time2;
  time_t limit = (time_t) (timeout * 1000.0);
  Phg_ret_inp_event *revt = &ret.data.inp_event;
  Phg_inp_event_data ed;

  ERR_SET_CUR_FUNC(PHG_ERH, Pfn_await_event);

  if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    ERR_REPORT(PHG_ERH, ERR3);
  }
  else {
    /* Process events one at time for each workstation
     * until one is available, or if the timeout expires
     */
    time = 0;
    do {
      phg_mtime(&time1);
      if (inp_dispatch_next(Pfn_await_event) == FALSE) {
        /* If there where no events sleep a while */
        phg_msleep(1);
      }
      inp_event_poll(&ret);
      phg_mtime(&time2);
      time += (time2 - time1);
      if (time >= limit) {
        break;
      }
    } while (revt->id.in_class == PIN_NONE);
    if (ret.err == 0) {
      *ws_id = revt->id.ws;
      *dev_class = revt->id.in_class;
      *in_num = revt->id.dev;
      switch (revt->id.in_class) {
      case PIN_STROKE:
        size = revt->data.stk.num_points * sizeof(Ppoint3);
        if (size > 0) {
          pts = (Ppoint3 *) malloc(size);
          if (pts == NULL) {
            ERR_REPORT(PHG_ERH, ERR900);
            revt->data.stk.num_points = 0;
          }
          else {
            memcpy(pts, revt->data.stk.points, size);
            revt->data.stk.points = pts;
          }
        }
        break;
      case PIN_PICK:
        if (revt->data.pik.status == PIN_STATUS_OK) {
          size = revt->data.pik.pick_path.depth *
            sizeof(Ppick_path_elem);
          if (size > 0) {
            path = (Ppick_path_elem *) malloc(size);
            if (path == NULL) {
              ERR_REPORT(PHG_ERH, ERR900);
              revt->data.pik.status = PIN_STATUS_NONE;
            }
            else {
              memcpy(path,
                     revt->data.pik.pick_path.path_list,
                     size);
              revt->data.pik.pick_path.path_list = path;
            }
          }
        }
        break;
      case PIN_STRING: {
        char    *str;
        if ( revt->data.str.length > 0 ) {
          str = malloc((unsigned)revt->data.str.length);
          strcpy(str, revt->data.str.string);
          ed.str.string = str;
        }
      } break;
      case PIN_VAL:
        ed.val = revt->data.val;
        break;
      case PIN_CHOICE:
        ed.chc.choice = revt->data.chc.choice;
        ed.chc.status = revt->data.chc.status;
        break;
      default:
        break;
      }
      PSL_CLEAR_CUR_EVENT(PHG_PSL); /* old one, we want to overwrite it */
      PSL_SET_CUR_EVENT_ID(PHG_PSL, revt->id);
      if (revt->id.in_class != PIN_NONE) {
        switch (revt->id.in_class) {
        case PIN_STROKE:
        case PIN_PICK:
          PSL_SET_CUR_EVENT_DATA(PHG_PSL, revt->data);
          break;
        case PIN_STRING:
        case PIN_VAL:
        case PIN_CHOICE:
          PSL_SET_CUR_EVENT_DATA(PHG_PSL, ed);
          break;
        case PIN_NONE:
        case PIN_LOC:
          break;
        }
      }
    }
    else {
      ERR_FLUSH(PHG_ERH);
    }
  }
}

