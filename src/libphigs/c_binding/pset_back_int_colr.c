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

#include <stdlib.h>
#include <string.h>

#include "phg.h"
#include "css.h"
#include "private/phgP.h"

/* Automatically generated prototypes */
extern void pfill_area_set3_data(
                          Pint fflag,
                          Pint eflag,
                          Pint vflag,
                          Pint colr_type,
                          Pfacet_data3 *fdata,
                          Pint nfa,
                          Pedge_data_list *edata,
                          Pfacet_vdata_list3 *vdata
                          );
extern void pfill_area_set_data(
                         Pint fflag,
                         Pint eflag,
                         Pint vflag,
                         Pint colr_type,
                         Pfacet_data3 *fdata,
                         Pint nfa,
                         Pedge_data_list *edata,
                         Pfacet_vdata_list3 *vdata
                         );
extern void pset_of_fill_area_set3_data(
                                 Pint fflag,
                                 Pint eflag,
                                 Pint vflag,
                                 Pint colr_type,
                                 Pint num_sets,
                                 Pfacet_data_arr3 *fdata,
                                 Pedge_data_list_list *edata,
                                 Pint_list_list *vlist,
                                 Pfacet_vdata_list3 *vdata
                                 );
extern void pset_back_int_style(
                         Pint_style int_style
                         );
extern void pset_back_int_style_ind(
                             Pint int_style_ind
                             );
extern void pset_int_colr(
                   Pgcolr *colr
                   );
extern void pset_back_int_colr(
                        Pgcolr *colr
                        );
extern void pset_line_colr(
                    Pgcolr *colr
                    );
extern void pset_marker_colr(
                      Pgcolr *colr
                      );
extern void pset_edge_colr(
                    Pgcolr *colr
                    );
extern void pset_text_colr(
                    Pgcolr *colr
                    );
extern void pset_light_src_state(
                          Pint_list *activation,
                          Pint_list *deactivation
                          );
extern void pset_int_shad_meth(
                        Pint shad_meth
                        );
extern void pset_back_int_shad_meth(
                             Pint shad_meth
                             );
extern void pset_refl_model(
                   Pint refl_model
                   );
extern void pset_refl_eqn(
                   Pint refl_equ
                   );
extern void pset_back_refl_eqn(
                        Pint refl_equ
                        );
extern void pset_refl_props(
                     Prefl_props *refl_props
                     );
extern void pset_back_refl_props(
                          Prefl_props *refl_props
                          );
extern void pset_face_disting_mode(
                            Pdisting_mode disting_mode
                            );
extern void pset_face_cull_mode(
                         Pcull_mode cull_mode
                         );
extern void pset_model_clip_ind(
                         Pclip_ind clipi
                         );
extern void  pset_model_clip_vol3 (
                            Pint op,
                            Phalf_space_list3 spacelist
                            );
extern void pset_alpha_channel(
                        Pfloat alpha
                        );


/**
 * \file pset_back_int_colr.c
 * \brief Creates a new element - Backface Color Attribute
 */
void pset_back_int_colr(
                        Pgcolr *colr
                        )
{
  Phg_args_add_el args;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_back_int_colr)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_BACK_INT_COLR;
      args.el_size = sizeof(Pgcolr);
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        memcpy(args.el_data, colr, args.el_size);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

