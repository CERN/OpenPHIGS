/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2026 Paramveer Singh
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

#ifndef _cb_internal_h
#define _cb_internal_h

#include "phg.h"
#include "private/phgP.h"
#include "ws_inp.h"

extern short int wsgl_use_shaders_settings;

extern Wst_input_wsdt* input_ws_open(
                                     Pint ws_id,
                                     Pint fn_id,
                                     Wst_phigs_dt **dtp,
                                     Pint *err_ind
                                     );
extern int check_event_class(
                             Pin_class dev_class,
                             Pint fn_id
                             );
extern int check_loc_data_record(
                                 Pint pet,
                                 Ploc_data3 *loc_data,
                                 Wst_phigs_dt *dt,
                                 Wst_defloc *ddt
                                 );
extern int check_pick_data_record(
                                  Pint pet,
                                  Ppick_data3 *stroke_data,
                                  Wst_phigs_dt *dt,
                                  Wst_defpick *ddt
                                  );
extern int check_stroke_data_record(
                                    Pint pet,
                                    Pstroke_data3 *stroke_data,
                                    Wst_phigs_dt *dt,
                                    Wst_defstroke *ddt
                                    );
extern int valid_ar_fname(
                          char *fname
                          );
extern void build_transform(
                            Ppoint *pt,                /* fixed point */
                            Pvec *shift,               /* shift vector */
                            Pfloat ang,                /* rotation angle */
                            Pvec *scl,                 /* scale vector */
                            Pmatrix m                  /* OUT transformation matrix */
                            );
extern void build_transform3(
                             Ppoint3 *pt,               /* fixed point */
                             Pvec3 *shift,              /* shift vector */
                             Pfloat ax,                 /* rotation angle X */
                             Pfloat ay,                 /* rotation angle Y */
                             Pfloat az,                 /* rotation angle Z */
                             Pvec3 *scl,                /* scale vector */
                             Pmatrix3 m                 /* OUT transformation matrix */
                             );
extern void inp_event_poll(
                           Phg_ret *ret
                           );
extern void inq_filter(
                       Phg_args_flt_type type,
                       Pint ws_id,
                       struct _Pstore *store,
                       Pint *err_ind,
                       Pfilter **filter
                       );
extern void pinq_table_indices(
                               Phg_args_rep_type type,
                               Pint ws_id,
                               Pint num_elems_appl_list,
                               Pint start_ind,
                               Pint *err_ind,
                               Pint_list *def_line_ind,
                               Pint *num_elems_impl_list
                               );
extern void request_device(
                           Pint ws_id,
                           Pint dev_num,
                           Phg_args_idev_class dev_class,
                           Phg_ret *ret
                           );
extern void sample_device(
                          Pint ws_id,
                          Pint dev_num,
                          Phg_args_idev_class dev_class,
                          Phg_ret *ret
                          );
extern void set_mode(
                     Pint ws_id,
                     Phg_args_idev_class dev_class,
                     Pint dev_num,
                     Pop_mode op_mode,
                     Pecho_switch echo_switch
                     );

extern int inp_dispatch_next(
                             Pint fn_id
                             );

#endif
