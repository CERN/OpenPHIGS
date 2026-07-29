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
#include <stdio.h>

#include "phg.h"
#include "css.h"
#include "private/phgP.h"

/* Automatically generated prototypes */
extern void padd_names_set(
                    Pint_list *names
                    );
extern void premove_names_set(
                       Pint_list *names
                       );
extern void pset_indiv_asf(
                    Paspect asf_id,
                    Pasf asf_source
                    );
extern void pset_local_tran3(
                      Pmatrix3 local_tran,
                      Pcompose_type compose_type
                      );
extern void pset_global_tran3(
                       Pmatrix3 global_tran
                       );
extern void pset_view_ind(
                   Pint view_ind
                   );
extern void ppolyline(
               Ppoint_list *point_list
               );
extern void ppolyline3(
                Ppoint_list3 *point_list
                );
extern void ppolymarker(
                 Ppoint_list *point_list
                 );
extern void ppolymarker3(
                  Ppoint_list3 *point_list
                  );
extern void pfill_area(
                Ppoint_list *point_list
                );
extern void pfill_area3(
                 Ppoint_list3 *point_list
                 );
extern void pfill_area_set(
                    Ppoint_list_list *point_list_list
                    );
extern void pfill_area_set3(
                     Ppoint_list_list3 *point_list_list
                     );
extern void plabel(
            Pint label_id
            );
extern void pset_pick_id(
                  Pint pick_id
                  );
extern void pset_hlhsr_id(
                   Pint hlhsr_id
                   );
extern void pset_int_ind(
                  Pint int_ind
                  );
extern void pset_int_colr_ind(
                       Pint colr_ind
                       );
extern void pset_back_int_colr_ind(
                       Pint colr_ind
                       );
extern void pset_int_style(
                    Pint_style int_style
                    );
extern void pset_int_style_ind(
                        Pint int_style_ind
                        );
extern void pset_line_ind(
                   Pint line_ind
                   );
extern void pset_line_colr_ind(
                        Pint colr_ind
                        );
extern void pset_linewidth(
                    Pfloat linewidth
                    );
extern void pset_linetype(
                   Pint linetype
                   );
extern void pset_marker_ind(
                     Pint marker_ind
                     );
extern void pset_marker_colr_ind(
                          Pint colr_ind
                          );
extern void pset_marker_size(
                      Pfloat marker_size
                      );
extern void pset_marker_type(
                      Pint marker_type
                      );
extern void pset_edge_ind(
                   Pint edge_ind
                   );
extern void pset_edge_colr_ind(
                        Pint colr_ind
                        );
extern void pset_edgetype(
                   Pint edgetype
                   );
extern void pset_edge_flag(
                    Pedge_flag edge_flag
                    );
extern void pset_edgewidth(
                    Pfloat edgewidth
                    );
extern void pset_char_ht(
                  Pfloat char_ht
                  );
extern void pset_char_expan(
                     Pfloat char_expan
                     );
extern void pset_char_space(
                     Pfloat char_space
                     );
extern void pset_char_up_vec(
                      Pvec *char_up_vec
                      );
extern void pset_anno_char_up_vec(
                           Pvec *char_up_vec
                           );
extern void pexec_struct(
                  Pint struct_id
                  );


/**
 * \file pset_linewidth.c
 * \brief Creates a new element - Line Width Attribute
 */
void pset_linewidth(
                    Pfloat linewidth
                    )
{
  Phg_args_add_el args;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_linewidth)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_LINEWIDTH;
      args.el_size = sizeof(Pfloat);
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        memcpy(args.el_data, &linewidth, args.el_size);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

