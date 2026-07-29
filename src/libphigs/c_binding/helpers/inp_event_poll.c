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
 * \file inp_event_poll.c
 * \brief Poll input events helper function
 */
void inp_event_poll(
                           Phg_ret *ret
                           )
{
  unsigned size;
  Ppick *pick;
  Phg_string *string;
  Sin_input_event *event;
  Pevent *ev_id = &ret->data.inp_event.id;
  Phg_inp_event_data *ed = &ret->data.inp_event.data;

  ret->err = 0;
  event = phg_sin_q_next_event(PHG_INPUT_Q);
  if (event != NULL) {
    if (SIN_Q_OVERFLOWED(PHG_INPUT_Q)) {
      ERR_BUF(PHG_ERH, ERR256);
    }
    ev_id->ws = event->wsid;
    ev_id->dev = event->dev_num;
    ev_id->in_class = event->dev_class;
    SIN_Q_SET_CUR_SIMUL_ID(PHG_INPUT_Q, event);

    switch (ev_id->in_class) {
    case PIN_LOC:
      ed->loc = event->data.locator.evt;
      break;
    case PIN_STROKE:
      size = event->data.stroke.evt.num_points * sizeof(Ppoint3);
      if ((size > 0) && (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, size))) {
        ERR_BUF(PHG_ERH, ERR900);
        ret->err = ERR900;
        free(ed->stk.points);
      }
      else {
        ed->stk = event->data.stroke.evt;
        if (size > 0) {
          memcpy(PHG_SCRATCH.buf, ed->stk.points, size);
          free(ed->stk.points);
          ed->stk.points = (Ppoint3 *) PHG_SCRATCH.buf;
        }
      }
      break;

    case PIN_PICK:
      pick = &event->data.pick.evt;
      ed->pik = *pick;
      if (pick->status == PIN_STATUS_OK) {
        size = pick->pick_path.depth * sizeof(Ppick_path_elem);
        if ((size > 0) && (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, size))) {
          ERR_BUF(PHG_ERH, ERR900);
          ret->err = ERR900;
          free(pick->pick_path.path_list);
        }
        else if (size > 0) {
          memcpy(PHG_SCRATCH.buf, pick->pick_path.path_list, size);
          free(pick->pick_path.path_list);
          ed->pik.pick_path.path_list = (Ppick_path_elem *)
            PHG_SCRATCH.buf;
        }
      }
      break;

    case PIN_VAL:
      ed->val = event->data.valuator.value;
      break;

    case PIN_CHOICE:
      ed->chc = event->data.choice.evt;
      break;

    case PIN_STRING:
      size = event->data.string.evt.length;
      if ((size > 0) && (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, size))) {
        ERR_BUF(PHG_ERH, ERR900);
        ret->err = ERR900;
        free(ed->stk.points);
      }
      else {
        ed->str = event->data.string.evt;
        if (size > 0) {
          memcpy(PHG_SCRATCH.buf, ed->str.string, size);
          free(ed->str.string);
          ed->str.string = (char *) PHG_SCRATCH.buf;
        }
      }
      break;
    default:
      break;
    }

    phg_sin_q_deque_event(PHG_INPUT_Q);
  }
  else {
    ev_id->in_class = PIN_NONE;
    if (SIN_Q_OVERFLOWED(PHG_INPUT_Q)) {
      SIN_Q_CLEAR_OVERFLOW(PHG_INPUT_Q);
    }
  }
}

