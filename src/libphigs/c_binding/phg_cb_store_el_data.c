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
#include <stdio.h>

#include "phg.h"
#include "private/phgP.h"
#include "private/cbP.h"

extern struct _Pstore *phg_cb_store_list;

/* Automatically generated prototypes */
extern int phg_cb_resize_store(
                        Pstore store,
                        Pint size,
                        Pint *err_ind
                        );
extern void phg_cb_destroy_all_stores(
                               void
                               );
extern int phg_cb_store_el_size(
                         Phg_elmt_info *el_info
                         );
extern void phg_cb_store_el_data(
                          Phg_elmt_info *el_info,
                          void *buf,
                          Pelem_data *ed
                          );


/**
 * \file phg_cb_store_el_data.c
 * \brief Store element data for inquiry function
 */
void phg_cb_store_el_data(
                          Phg_elmt_info *el_info,
                          void *buf,
                          Pelem_data *ed
                          )
{
  Pint i;
  Pint *idata;
  Ppoint *pdata;
  Ppoint3 *p3data;

   switch(el_info->elementType) {
   case PELEM_ADD_NAMES_SET:
   case PELEM_REMOVE_NAMES_SET:
     idata = (Pint *) &el_info[1];
     ed->int_list.num_ints = *idata;
     ed->int_list.ints = (Pint *) &idata[1];
     break;

   case PELEM_POLYLINE:
   case PELEM_POLYMARKER:
   case PELEM_FILL_AREA:
     idata = (Pint *) &el_info[1];
     ed->point_list.num_points = *idata;
     ed->point_list.points = (Ppoint *) &idata[1];
     break;

   case PELEM_POLYLINE3:
   case PELEM_POLYMARKER3:
   case PELEM_FILL_AREA3:
     idata = (Pint *) &el_info[1];
     ed->point_list3.num_points = *idata;
     ed->point_list3.points = (Ppoint3 *) &idata[1];
     break;

   case PELEM_FILL_AREA_SET:
     idata = (Pint *) &el_info[1];
     ed->point_list_list.num_point_lists = *idata;
     idata = (Pint *) &idata[1];
     ed->point_list_list.point_lists = (Ppoint_list *) buf;
     for (i = 0; i < ed->point_list_list.num_point_lists; i++) {
       ed->point_list_list.point_lists[i].num_points = *idata;
       ed->point_list_list.point_lists[i].points = (Ppoint *) &idata[1];
       idata = (Pint *) &ed->point_list_list.point_lists[i].points[
                                                                   ed->point_list_list.point_lists[i].num_points];
     }
     break;

   case PELEM_FILL_AREA_SET3:
     idata = (Pint *) &el_info[1];
     ed->point_list_list3.num_point_lists = *idata;
     idata = (Pint *) &idata[1];
     ed->point_list_list3.point_lists = (Ppoint_list3 *) buf;
     for (i = 0; i < ed->point_list_list3.num_point_lists; i++) {
       ed->point_list_list3.point_lists[i].num_points = *idata;
       ed->point_list_list3.point_lists[i].points = (Ppoint3 *) &idata[1];
       idata = (Pint *) &ed->point_list_list3.point_lists[i].points[
                                                                    ed->point_list_list3.point_lists[i].num_points];
     }
     break;

   case PELEM_FILL_AREA_SET3_DATA:
     /* TODO */
     break;

   case PELEM_SET_OF_FILL_AREA_SET3_DATA:
     /* TODO */
     break;

   case PELEM_TEXT:
     {
       Ppoint *pdata = (Ppoint *) &el_info[1];
       memcpy(&ed->text.pos, pdata, sizeof(Ppoint));
       ed->text.char_string = (char *) &pdata[1];
     } break;

   case PELEM_LINEWIDTH:
   case PELEM_MARKER_SIZE:
   case PELEM_EDGEWIDTH:
   case PELEM_CHAR_HT:
   case PELEM_CHAR_EXPAN:
   case PELEM_CHAR_SPACE:
     memcpy(&ed->float_data, &el_info[1], sizeof(Pfloat));
     break;

   case PELEM_INDIV_ASF:
     memcpy(&ed->asf_info, &el_info[1], sizeof(Pasf_info));
     break;

   case PELEM_TEXT_ALIGN:
     memcpy(&ed->text_align, &el_info[1], sizeof(Ptext_align));
     break;

   case PELEM_CHAR_UP_VEC:
     memcpy(&ed->vec, &el_info[1], sizeof(Pvec));
     break;

   case PELEM_LOCAL_MODEL_TRAN3:
     phg_get_local_tran3(&ed->local_tran3, (Pfloat *) &el_info[1]);
     break;

   case PELEM_GLOBAL_MODEL_TRAN3:
     phg_mat_pack(ed->global_tran3, (Pfloat *) &el_info[1]);
     break;

   case PELEM_ANNO_TEXT_REL3:
     {
       p3data = (Ppoint3 *) &el_info[1];
       memcpy(&ed->anno_text_rel3.ref_point, p3data, sizeof(Ppoint3));
       Pvec3 *o3data = (Pvec3 *) &p3data[1];
       memcpy(&ed->anno_text_rel3.offset, o3data, sizeof(Pvec3));
       ed->anno_text_rel3.char_string = (char *) &o3data[1];
     };
     break;

   case PELEM_ANNO_TEXT_REL:
     {
       pdata = (Ppoint *) &el_info[1];
       memcpy(&ed->anno_text_rel.ref_point, pdata, sizeof(Ppoint));
       Pvec *odata = (Pvec *) &pdata[1];
       memcpy(&ed->anno_text_rel.offset, odata, sizeof(Pvec));
       ed->anno_text_rel.char_string = (char *) &odata[1];
     }
     break;

   case PELEM_INT_COLR:
   case PELEM_BACK_INT_COLR:
   case PELEM_LINE_COLR:
   case PELEM_MARKER_COLR:
   case PELEM_EDGE_COLR:
   case PELEM_TEXT_COLR:
     memcpy(&ed->colr, &el_info[1], sizeof(Pgcolr));
     break;

   case PELEM_INT_COLR_IND:
   case PELEM_EDGE_COLR_IND:
   case PELEM_MARKER_COLR_IND:
   case PELEM_TEXT_COLR_IND:
     memcpy(&ed->int_data, &el_info[1], sizeof(Pint));
     break;

   case PELEM_REFL_PROPS:
   case PELEM_BACK_REFL_PROPS:
     memcpy(&ed->props, &el_info[1], sizeof(Prefl_props));
     break;

      case PELEM_LIGHT_SRC_STATE:
        /* TODO */
        break;
   case PELEM_LABEL:
  memcpy(&ed->int_data, &el_info[1], sizeof(Pint));
  break;
   default:
     memcpy(&ed->int_data, &el_info[1], sizeof(Pint));
     break;
   }
}
