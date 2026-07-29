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
 * \file pset_of_fill_area_set3_data.c
 * \brief Creates a new element - Set of fill area set with data 3D
 */
void pset_of_fill_area_set3_data(
                                 Pint fflag,
                                 Pint eflag,
                                 Pint vflag,
                                 Pint colr_type,
                                 Pint num_sets,
                                 Pfacet_data_arr3 *fdata,
                                 Pedge_data_list_list *edata,
                                 Pint_list_list *vlist,
                                 Pfacet_vdata_list3 *vdata
                                 )
{
  Phg_args_add_el args;
  Pint i, j;
  unsigned facet_size, vertex_size;
  Pint *data;
  char *tp;
  Pint num_lists, num_vertices;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_of_fill_area_set3_data)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      switch (fflag) {
      case PFACET_COLOUR:
        facet_size = sizeof(Pcoval);
        break;

      case PFACET_NORMAL:
        facet_size = sizeof(Pvec3);
        break;

      case PFACET_COLOUR_NORMAL:
        facet_size = sizeof(Pconorm3);
        break;

      default:
        facet_size = 0;
        break;
      }

      switch (vflag) {
      case PVERT_COORD:
        vertex_size = sizeof(Ppoint3);
        break;

      case PVERT_COORD_COLOUR:
        vertex_size = sizeof(Pptco3);
        break;

      case PVERT_COORD_NORMAL:
        vertex_size = sizeof(Pptnorm3);
        break;

      case PVERT_COORD_COLOUR_NORMAL:
        vertex_size = sizeof(Pptconorm3);
        break;

      default:
        vertex_size = 0;
        break;
      }

      args.el_type = PELEM_SET_OF_FILL_AREA_SET3_DATA;
      args.el_size = 5 * sizeof(Pint) + num_sets * facet_size;

      if (eflag == PEDGE_VISIBILITY) {
        for (i = 0; i < num_sets; i++) {
          args.el_size += sizeof(Pint);      /* Pint num_lists */
          for (j = 0; j < edata[i].num_lists; j++) {
            args.el_size += sizeof(Pint);   /* Pint num_edges */
            args.el_size += edata[i].edgelist[j].num_edges *
              sizeof(Pedge_flag);
          }
        }
      }

      for (i = 0; i < num_sets; i++) {
        args.el_size += sizeof(Pint);      /* Pint num_lists */
        for (j = 0; j < vlist[i].num_lists; j++) {
          args.el_size += sizeof(Pint);   /* Pint num_ints */
          args.el_size += vlist[i].lists[j].num_ints * sizeof(Pint);
        }
      }

      args.el_size += sizeof(Pint);         /* Pint num_vertices */
      args.el_size += vdata->num_vertices * vertex_size;

      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;

        data[0] = fflag;
        data = &data[1];
        data[0] = eflag;
        data = &data[1];
        data[0] = vflag;
        data = &data[1];
        data[0] = colr_type;
        data = &data[1];
        data[0] = num_sets;
        tp = (char *) &data[1];

        switch(fflag) {
        case PFACET_COLOUR:
          memcpy(tp, fdata->colrs, sizeof(Pcoval) * num_sets);
          tp += sizeof(Pcoval) * num_sets;
          break;

        case PFACET_NORMAL:
          memcpy(tp, fdata->norms, sizeof(Pvec3) * num_sets);
          tp += sizeof(Pvec3) * num_sets;
          break;

        case PFACET_COLOUR_NORMAL:
          memcpy(tp, fdata->conorms, sizeof(Pconorm3) * num_sets);
          tp += sizeof(Pconorm3) * num_sets;
          break;

        default:
          break;
        }

        if (eflag == PEDGE_VISIBILITY) {
          for (i = 0; i < num_sets; i++) {
            data = (Pint *) tp;
            num_lists = edata[i].num_lists;
            data[0] = num_lists;
            tp = (char *) &data[1];
            for (j = 0; j < num_lists; j++) {
              num_vertices = edata[i].edgelist[j].num_edges;
              data = (Pint *) tp;
              data[0] = num_vertices;
              tp = (char *) &data[1];
              memcpy(tp, edata[i].edgelist[j].edgedata.edges,
                     sizeof(Pedge_flag) * num_vertices);
              tp += sizeof(Pedge_flag) * num_vertices;
            }
          }
        }

        for (i = 0; i < num_sets; i++) {
          data = (Pint *) tp;
          num_lists = vlist[i].num_lists;
          data[0] = num_lists;
          tp = (char *) &data[1];
          for (j = 0; j < num_lists; j++) {
            num_vertices = vlist[i].lists[j].num_ints;
            data = (Pint *) tp;
            data[0] = num_vertices;
            tp = (char *) &data[1];
            memcpy(tp, vlist[i].lists[j].ints,
                   sizeof(Pint) * num_vertices);
            tp += sizeof(Pint) * num_vertices;
          }
        }

        num_vertices = vdata->num_vertices;

        data = (Pint *) tp;
        data[0] = num_vertices;
        tp = (char *) &data[1];

        switch (vflag) {
        case PVERT_COORD:
          memcpy(tp, vdata->vertex_data.points,
                 num_vertices * sizeof(Ppoint3));
          break;

        case PVERT_COORD_COLOUR:
          memcpy(tp, vdata->vertex_data.ptcolrs,
                 num_vertices * sizeof(Pptco3));
          break;

        case PVERT_COORD_NORMAL:
          memcpy(tp, vdata->vertex_data.ptnorms,
                 num_vertices * sizeof(Pptnorm3));
          break;

        case PVERT_COORD_COLOUR_NORMAL:
          memcpy(tp, vdata->vertex_data.ptconorms,
                 num_vertices * sizeof(Pptconorm3));
          break;

        default:
          break;
        }

        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

