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
 * \file pinq_line_rep.c
 * \brief Get workstation line representation
 */
void pinq_line_rep(
                   Pint ws_id,
                   Pint index,
                   Pinq_type type,
                   Pint *err_ind,
                   Pline_bundle *line_rep
                   )
{
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Phg_ret ret;

  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR3;
  }
  else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    *err_ind = ERR3;
  }
  else {
    wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
    if (wsinfo == NULL) {
      *err_ind = ERR54;
    }
    else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!(dt->ws_category == PCAT_OUT ||
            dt->ws_category == PCAT_TGA ||
            dt->ws_category == PCAT_PNG ||
            dt->ws_category == PCAT_PNGA ||
            dt->ws_category == PCAT_EPS ||
            dt->ws_category == PCAT_PDF ||
            dt->ws_category == PCAT_SVG ||
            dt->ws_category == PCAT_OBJ ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else if (index < 1) {
        *err_ind = ERR100;
      }
      else {
        wsh = PHG_WSID(ws_id);
        (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_EXTLNREP,
                                   &ret);
        if (ret.err) {
          *err_ind = ret.err;
        }
        else if (ret.data.rep.extlnrep.colr.type != PINDIRECT) {
          *err_ind = ERR134;
        }
        else {
          line_rep->type = ret.data.rep.extlnrep.type;
          line_rep->width = ret.data.rep.extlnrep.width;
          line_rep->colr_ind = ret.data.rep.extlnrep.colr.val.ind;
          *err_ind = 0;
        }
      }
    }
  }
}

