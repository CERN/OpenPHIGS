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
#include <string.h>
#include <math.h>
#include <png.h>
#ifdef GLEW
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif
#include <gl2ps.h>

#include "phg.h"
#include "css.h"
#include "ws.h"
#include "private/phgP.h"
#include "private/cbP.h"
#include "private/wsglP.h"
#include "private/wsxP.h"
#include "phconf.h"

short int wsgl_use_shaders_settings;

/* Automatically generated prototypes */
extern void popen_ws(
              Pint ws_id,
              Phg_args_conn_info *conn_id,
              Pint ws_type
              );
extern void pclose_ws(
               Pint ws_id
               );
extern void pset_ws_vp(
                Pint ws_id,
                Plimit *viewport
                );
extern void pset_ws_vp3(
                 Pint ws_id,
                 Plimit3 *viewport
                 );
extern void pset_ws_win(
                 Pint ws_id,
                 Plimit *window
                 );
extern void pset_ws_win3(
                  Pint ws_id,
                  Plimit3 *window
                  );
extern void pset_invis_filter(
                       Pint ws_id,
                       Pfilter *filter
                       );
extern void pset_hlhsr_mode(
                     Pint ws_id,
                     Pint hlhsr_mode
                     );
extern void pset_line_rep(
                   Pint ws_id,
                   Pint line_ind,
                   Pline_bundle *line_bundle
                   );
extern void pset_marker_rep(
                     Pint ws_id,
                     Pint marker_ind,
                     Pmarker_bundle *marker_bundle
                     );
extern void pset_text_rep(
                   Pint ws_id,
                   Pint text_ind,
                   Ptext_bundle *text_bundle
                   );
extern void pset_edge_rep(
                   Pint ws_id,
                   Pint edge_ind,
                   Pedge_bundle *edge_bundle
                   );
extern void pset_int_rep(
                  Pint ws_id,
                  Pint int_ind,
                  Pint_bundle *int_bundle
                  );
extern void ppost_struct(
                  Pint ws_id,
                  Pint struct_id,
                  Pfloat priority
                  );
extern void punpost_struct(
                    Pint ws_id,
                    Pint struct_id
                    );
extern void punpost_all_structs(
                         Pint ws_id
                         );
extern void predraw_all_structs(
                         Pint ws_id,
                         Pctrl_flag ctrl_flag
                         );
extern void pupd_ws(
             Pint ws_id,
             Pregen_flag regen_flag
             );
extern void pset_disp_upd_st(
                      Pint ws_id,
                      Pdefer_mode def_mode,
                      Pmod_mode mod_mode
                      );
extern void pset_view_tran_in_pri(
                           Pint ws_id,
                           Pint view_ind,
                           Pint ref_view_ind,
                           Prel_pri rel_pri
                           );
extern void pset_light_src_rep(
                        Pint ws_id,
                        Pint light_src_ind,
                        Plight_src_bundle *light_src_rep
                        );
extern void pset_colr_model(
                     Pint ws_id,
                     Pint model
                     );
extern void pset_colr_rep(
                   Pint ws_id,
                   Pint ind,
                   Pcolr_rep *rep
                   );
extern void pinq_ws_st(
                Pws_st *ws_state
                );
extern void pinq_ws_conn_type(
                       Pint ws_id,
                       Pstore store,
                       Pint *err_ind,
                       void **conn_id,
                       Pint *ws_type
                       );
extern void pinq_open_wss(
                   Pint num_elems_appl_list,
                   Pint start_ind,
                   Pint *err_ind,
                   Pint_list *open_ws_ids,
                   Pint *num_elems_impl_list
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
extern void pinq_list_view_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *view_inds,
                         Pint *num_elems_impl_list
                         );
extern void pinq_list_line_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *def_line_ind,
                         Pint *num_elems_impl_list
                         );
extern void pinq_list_marker_inds(
                           Pint ws_id,
                           Pint num_elems_appl_list,
                           Pint start_ind,
                           Pint *err_ind,
                           Pint_list *def_marker_ind,
                           Pint *num_elems_impl_list
                           );
extern void pinq_list_text_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *def_text_ind,
                         Pint *num_elems_impl_list
                         );
extern void pinq_list_int_inds(
                        Pint ws_id,
                        Pint num_elems_appl_list,
                        Pint start_ind,
                        Pint *err_ind,
                        Pint_list *def_int_ind,
                        Pint *num_elems_impl_list
                        );
extern void pinq_list_edge_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *def_edge_ind,
                         Pint *num_elems_impl_list
                         );
extern void pinq_list_colr_inds(
                         Pint ws_id,
                         Pint num_elems_appl_list,
                         Pint start_ind,
                         Pint *err_ind,
                         Pint_list *colr_ind,
                         Pint *num_elems_impl_list
                         );
extern void pinq_line_rep(
                   Pint ws_id,
                   Pint index,
                   Pinq_type type,
                   Pint *err_ind,
                   Pline_bundle *line_rep
                   );
extern void pinq_marker_rep(
                     Pint ws_id,
                     Pint index,
                     Pinq_type type,
                     Pint *err_ind,
                     Pmarker_bundle *marker_rep
                     );
extern void pinq_text_rep(
                   Pint ws_id,
                   Pint index,
                   Pinq_type type,
                   Pint *err_ind,
                   Ptext_bundle *text_rep
                   );
extern void pinq_int_rep(
                  Pint ws_id,
                  Pint index,
                  Pinq_type type,
                  Pint *err_ind,
                  Pint_bundle *int_rep
                  );
extern void pinq_edge_rep(
                   Pint ws_id,
                   Pint index,
                   Pinq_type type,
                   Pint *err_ind,
                   Pedge_bundle *edge_rep
                   );
extern void pinq_colr_rep(
                   Pint ws_id,
                   Pint colr_ind,
                   Pinq_type type,
                   Pint *err_ind,
                   Pcolr_rep *colr_rep
                   );
extern void inq_filter(
                       Phg_args_flt_type type,
                       Pint ws_id,
                       struct _Pstore *store,
                       Pint *err_ind,
                       Pfilter **filter
                       );
extern void pinq_invis_filter(
                       Pint ws_id,
                       Pstore store,
                       Pint *err_ind,
                       Pfilter **invis_filter
                       );
extern void pmessage(
              Pint ws_id,
              char* message
              );


/**
 * \file pinq_invis_filter.c
 * \brief Get workstation invisibility filter
 */
void pinq_invis_filter(
                       Pint ws_id,
                       Pstore store,
                       Pint *err_ind,
                       Pfilter **invis_filter
                       )
{
  inq_filter(PHG_ARGS_FLT_INVIS, ws_id, ((struct _Pstore *) store),
             err_ind, invis_filter);
}

/***********************************
 * predefine 125 colors
 * in RGBA mode
 *    - prepeated with offset 200*i and increasing transparency
 *    - dito for any already existing first 16 colors
 **********************************/
void pxset_color_map(Pint ws_id){
  int i, j, k, l;
  int n = 5;
  int index = 0;
  int offset = 16;
  float delta_n = 1.0/(n-1);
  Pcolr_rep rep;
  Ws_handle wsh;
  Pgcolr gcolr;
  Pfloat def_alpha[n+1];
  memcpy(def_alpha, (float[]) { 1.0, 0.8, 0.6, 0.4, 0.3, 0.1}, sizeof def_alpha);

  wsh = PHG_WSID(ws_id);
  switch (wsh->current_colour_model){
  case PINDIRECT:
    break;
  case PMODEL_RGB:
    for (i=0; i<n; i++){
      for (j=0; j<n; j++){
        for (k=0; k<n; k++){
          rep.rgb.red   = i*delta_n;
          rep.rgb.green = j*delta_n;
          rep.rgb.blue  = k*delta_n;
#ifdef DEBUG
          printf("Defining color index %d as RGB %f %f %f\n", offset+index, rep.rgb.red, rep.rgb.green, rep.rgb.blue);
#endif
          pset_colr_rep(ws_id, offset+index, &rep);
          index += 1;
        }
      }
    }
    break;
  case PMODEL_RGBA:
    for (i=0; i<=n; i++){
      index = 0;
      for (j=0; j<n; j++){
        for (k=0; k<n; k++){
          for (l=0; l<n; l++){
            rep.rgba.red   = j*delta_n;
            rep.rgba.green = k*delta_n;
            rep.rgba.blue  = l*delta_n;
            rep.rgba.alpha = def_alpha[i];
            pset_colr_rep(ws_id, offset+index+200*i, &rep);
#ifdef DEBUGA
            printf("Defining color index %d as RGBA %f %f %f %f\n",
                   offset+index+200*i,
                   rep.rgba.red, rep.rgba.green, rep.rgba.blue, rep.rgba.alpha);
#endif
            index += 1;
          }
        }
      }
    }
    /* Redefine any existing colors with transparency */
    for (i=0;i<=16;i++){
      phg_get_colr_ind(wsh, &gcolr, i);
      switch (gcolr.type){
      case PINDIRECT:
        break;
      case PMODEL_RGB:
        break;
      case PMODEL_RGBA:
        for (j=1;j<=n;j++){
          rep.rgba.red   = gcolr.val.general.x;
          rep.rgba.green = gcolr.val.general.y;
          rep.rgba.blue  = gcolr.val.general.z;
          rep.rgba.alpha = def_alpha[j];
#ifdef DEBUGA
          printf("Re-defining color index %d as RGBA %f %f %f %f\n",
                 i+200*j, rep.rgba.red, rep.rgba.green, rep.rgba.blue, rep.rgba.alpha);
#endif
          pset_colr_rep(ws_id, i+200*j, &rep);
        }
        break;
      default:
#ifdef DEBUGA
        printf("WARNING in pxset_color_map: Skipping non-exiting color index %d\n", i);
#endif
        break;
      }
    }
    break;
  default:
    printf("WARNING in pxset_color_map: unknown color model %d. Ignoring function.\n", gcolr.type);
  }
}

