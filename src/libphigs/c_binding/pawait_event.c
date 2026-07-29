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
#include "private/sinqP.h"
#include "private/wsxP.h"

/* Automatically generated prototypes */
extern Wst_input_wsdt* input_ws_open(
                                     Pint ws_id,
                                     Pint fn_id,
                                     Wst_phigs_dt **dtp,
                                     Pint *err_ind
                                     );
extern int check_loc_data_record(
                                 Pint pet,
                                 Ploc_data3 *loc_data,
                                 Wst_phigs_dt *dt,
                                 Wst_defloc *ddt
                                 );
extern void pinit_loc3(
                Pint ws_id,
                Pint loc_num,
                Pint init_view_ind,
                Ppoint3 *init_loc_pos,
                Pint pet,
                Plimit3 *echo_vol,
                Ploc_data3 *loc_data
                );
extern int check_stroke_data_record(
                                    Pint pet,
                                    Pstroke_data3 *stroke_data,
                                    Wst_phigs_dt *dt,
                                    Wst_defstroke *ddt
                                    );
extern void pinit_stroke3(
                   Pint ws_id,
                   Pint stroke_num,
                   Pint init_view_ind,
                   Ppoint_list3 *init_stroke,
                   Pint pet,
                   Plimit3 *echo_vol,
                   Pstroke_data3 *stroke_data
                   );
extern int check_pick_data_record(
                                  Pint pet,
                                  Ppick_data3 *stroke_data,
                                  Wst_phigs_dt *dt,
                                  Wst_defpick *ddt
                                  );
extern void pinit_pick3(
                 Pint ws_id,
                 Pint pick_num,
                 Pin_status init_status,
                 Ppick_path *init_pick,
                 Pint pet,
                 Plimit3 *echo_vol,
                 Ppick_data3 *pick_data,
                 Ppath_order order
                 );
extern void pinit_string3(
                   Pint ws_id,
                   Pint string_dev,
                   char * init_string, //fixme: should be const
                   Pint pet,
                   Plimit3 * echo_vol,
                   Pstring_data3 * string_data
                   );
extern void pinit_string(
                  Pint ws_id,
                  Pint string_dev,
                  char * init_string,
                  Pint pet,
                  Plimit * area,
                  Pstring_data *string_data
                  );
extern void pinit_choice3(
                   Pint ws_id,
                   Pint choice_dev,
                   Pin_status init_status,
                   Pint init_choice,
                   Pint pet,
                   Plimit3 * echo_volume,
                   Pchoice_data3 *choice_data_rec
                   );
extern void pinit_val3(
                Pint ws_id,
                Pint val_dev,
                Pfloat init_value,
                Pint pet,
                Plimit3 *echo_volume,
                Pval_data *val_data_rec
                );
extern void pset_pick_filter(
                      Pint ws_id,
                      Pint pick_num,
                      Pfilter *filter
                      );
extern void pset_highl_filter(
                       Pint ws_id,
                       Pfilter *filter
                       );
extern void set_mode(
                     Pint ws_id,
                     Phg_args_idev_class dev_class,
                     Pint dev_num,
                     Pop_mode op_mode,
                     Pecho_switch echo_switch
                     );
extern void pset_loc_mode(
                   Pint ws_id,
                   Pint loc_num,
                   Pop_mode op_mode,
                   Pecho_switch echo_switch
                   );
extern void pset_stroke_mode(
                      Pint ws_id,
                      Pint stroke_num,
                      Pop_mode op_mode,
                      Pecho_switch echo_switch
                      );
extern void pset_pick_mode(
                    Pint ws_id,
                    Pint pick_num,
                    Pop_mode op_mode,
                    Pecho_switch echo_switch
                    );
extern void pset_string_mode(
                      Pint ws_id,
                      Pint string_dev,
                      Pop_mode op_mode,
                      Pecho_switch echo_switch
                      );
extern void pset_choice_mode(
                      Pint ws_id,
                      Pint choice_dev,
                      Pop_mode op_mode,
                      Pecho_switch echo_switch
                      );
extern void pset_val_mode(
                   Pint ws_id,
                   Pint val_dev,
                   Pop_mode op_mode,
                   Pecho_switch echo_switch
                   );
extern void sample_device(
                          Pint ws_id,
                          Pint dev_num,
                          Phg_args_idev_class dev_class,
                          Phg_ret *ret
                          );
extern void psample_loc(
                 Pint ws_id,
                 Pint loc_num,
                 Pint *view_ind,
                 Ppoint *loc_pos
                 );
extern void psample_loc3(
                  Pint ws_id,
                  Pint loc_num,
                  Pint *view_ind,
                  Ppoint3 *loc_pos
                  );
extern void psample_stroke(
                    Pint ws_id,
                    Pint stroke_num,
                    Pint *view_ind,
                    Ppoint_list *stroke
                    );
extern void psample_stroke3(
                     Pint ws_id,
                     Pint stroke_num,
                     Pint *view_ind,
                     Ppoint_list3 *stroke
                     );
extern void psample_pick(
                  Pint ws_id,
                  Pint pick_num,
                  Pint depth,
                  Pin_status *pick_in_status,
                  Ppick_path *pick
                  );
extern void psample_string(
                    Pint ws_id,
                    Pint string_dev,
                    char* string
                    );
extern int inp_dispatch_next(
                      Pint fn_id
                      );
extern void inp_event_poll(
                           Phg_ret *ret
                           );
extern void pawait_event(
                  Pfloat timeout,
                  Pint *ws_id,
                  Pin_class *dev_class,
                  Pint *in_num
                  );
extern int check_event_class(
                             Pin_class dev_class,
                             Pint fn_id
                             );
extern void pget_loc(
              Pint *view_ind,
              Ppoint *loc_pos
              );
extern void pget_loc3(
               Pint *view_ind,
               Ppoint3 *loc_pos
               );
extern void pget_stroke(
                 Pint *view_ind,
                 Ppoint_list *stroke
                 );
extern void pget_stroke3(
                  Pint *view_ind,
                  Ppoint_list3 *stroke
                  );
extern void pget_pick(
               Pint depth,
               Pin_status *in_status,
               Ppick_path *pick
               );
extern void pget_val(
              Pfloat *val
              );
extern void pget_choice(
                 Pin_status *in_status,
                 Pint *choice
                 );
extern void request_device(
                           Pint ws_id,
                           Pint dev_num,
                           Phg_args_idev_class dev_class,
                           Phg_ret *ret
                           );
extern void preq_loc3(
               Pint ws_id,
               Pint loc_num,
               Pin_status *in_status,
               Pint *view_ind,
               Ppoint3 *loc_pos
               );
extern void preq_stroke3(
                  Pint ws_id,
                  Pint stroke_num,
                  Pin_status *in_status,
                  Pint *view_ind,
                  Ppoint_list3 *stroke
                  );
extern void preq_pick(
               Pint ws_id,
               Pint pick_num,
               Pint depth,
               Pin_status *status,
               Ppick_path *pick
               );
extern void preq_string(
                 Pint ws_id,
                 Pint string_dev,
                 Pin_status *status,
                 char *string
                 );
extern void preq_choice(
                 Pint ws_id,
                 Pint choice_dev,
                 Pin_status *status,
                 Pint *choice
                 );
extern void preq_valuator(
                   Pint ws_id,
                   Pint val_dev,
                   Pin_status *status,
                   Pfloat *value
                   );
extern void pflush_events(
                   Pint        ws_id,     /* workstation identifier       */
                   Pin_class   inp_class,  /* device class */
                   Pint        dev    /* logical input device number  */
                   );


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

