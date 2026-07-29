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
#include "css.h"
#include "private/phgP.h"
#include "private/cbP.h"
#include "util/ftn.h"

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(popst)(;
extern FTN_SUBROUTINE(pclst)(;
extern FTN_SUBROUTINE(pdel)(void);
extern FTN_SUBROUTINE(pdst)(;
extern FTN_SUBROUTINE(posep)(;
extern FTN_SUBROUTINE(psep)(;
extern FTN_SUBROUTINE(pseplb)(;
extern FTN_SUBROUTINE(psedm)(;
extern FTN_SUBROUTINE(pqedm)(;
extern FTN_SUBROUTINE(pels)(;
extern FTN_SUBROUTINE(pqstst)(;
extern FTN_SUBROUTINE(pqeco)(;
extern FTN_SUBROUTINE(dumpstr)(FTN_INTEGER(strid));
extern FTN_SUBROUTINE(pcelst)(FTN_INTEGER(strid));
extern FTN_SUBROUTINE(pdas)();
extern FTN_SUBROUTINE(pqstrs)(Pint* strsta);


/**
 * \file pqeco.c
 * \brief Inquire element content
 *
 * \return error indicator, num ints, ints array, num float, float array, num strings, array of string lengths, character string entries
 */
FTN_SUBROUTINE(pqeco)(
                      FTN_INTEGER(strid),
                      FTN_INTEGER(elenum),
                      FTN_INTEGER(iil1),
                      FTN_INTEGER(irl1),
                      FTN_INTEGER(isl1),
                      FTN_INTEGER(err_ind),
                      FTN_INTEGER(il),
                      FTN_INTEGER_ARRAY(ia),
                      FTN_INTEGER(rl),
                      FTN_REAL_ARRAY(ra),
                      FTN_INTEGER(sl),
                      FTN_INTEGER_ARRAY(lstr),
                      char* str
                      ) {
  int struct_id = FTN_INTEGER_GET(strid);
  int elem_num = FTN_INTEGER_GET(elenum);
  int iil = FTN_INTEGER_GET(iil1);
  int irl = FTN_INTEGER_GET(irl1);
  int isl = FTN_INTEGER_GET(isl1);
  Pelem_data *elem_data;
  Pstore store;
  Phg_ret ret;
  Phg_elmt_info *el_info;
  int size;
  Pelem_type elem_type;
  Struct_handle structp;
  *err_ind = 0;
  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    printf("ERROR in PQECO Pfn_INQUIRY");
    *err_ind = ERR5;
  }
  else {
    ret.err = 0;
    phg_css_inq_el_type_size(PHG_CSS, struct_id, elem_num, &ret);
    if (ret.err != 0) {
      if ( !(structp = CSS_STRUCT_EXISTS(PHG_CSS, struct_id)) ) {
        printf("Could not find struct_id %d\n", struct_id);
      } else {
        printf("Dumping structure %d\n", struct_id);
        phg_css_print_struct(structp, 0);
      }
      printf("PQECO ERROR: cannot estimate element type and size %d\n", elem_num);
      *err_ind = ret.err;
    } else {
      elem_type = ret.data.el_type_size.type;
      phg_css_inq_el_content(PHG_CSS, struct_id, elem_num, &ret);
      if (ret.err == 0) {
        if (ret.data.el_info.op != PELEM_NIL) {
          el_info = ret.data.el_info.el_head;
          pcreate_store(err_ind, &store);
          size = phg_cb_store_el_size(el_info);
          if (phg_cb_resize_store(store, size, err_ind)) {
            phg_cb_store_el_data(el_info, store->buf,
                                 &store->data.elem_data);
            elem_data = &(store->data.elem_data);
          }
          *err_ind = ret.err;
          *il = 0;
          *rl = 0;
          *sl = 0;
          if (*err_ind == 0){
            if (iil == 1){
              switch (elem_type) {
              case PELEM_INT_COLR_IND:
              case PELEM_LINE_COLR_IND:
              case PELEM_MARKER_COLR_IND:
              case PELEM_EDGE_COLR_IND:
              case PELEM_TEXT_COLR_IND:
              case PELEM_LABEL:
                *ia = elem_data->int_data;
                *il = 1;
                break;
              case PELEM_INT_COLR:
              case PELEM_BACK_INT_COLR:
              case PELEM_LINE_COLR:
              case PELEM_MARKER_COLR:
              case PELEM_EDGE_COLR:
              case PELEM_TEXT_COLR:
                if (elem_data->colr.type == PINDIRECT){
                  *ia = elem_data->colr.val.ind;
                  *il = 1;
                } else {
                  *err_ind=4;
                  printf("ERROR in PQECO: Integer requested but colr type is not indirect: %d elem_type %d\n", (int)elem_data->colr.type, (int)elem_type);
                }
#ifdef DEBUG
                printf("PQECO returning indirect color %d\n", *ia);
#endif
                break;
              default:
                css_print_eltype(elem_type);
                printf("ERROR in PQECO: unknown element type %d. Ignoring function\n", (int)elem_type);
                *err_ind = 2;
              }
            } else if (irl == 1){
              *rl = 1;
              *ra = elem_data->float_data;
            } else if (isl > 0) {
              printf("ERROR in PQECO: Strings not yet implemented. Ignoring function\n");
              *err_ind = 3;
            } else {
              switch (elem_type) {
              case PELEM_INT_COLR:
              case PELEM_BACK_INT_COLR:
              case PELEM_LINE_COLR:
              case PELEM_MARKER_COLR:
              case PELEM_EDGE_COLR:
              case PELEM_TEXT_COLR:
		switch (elem_data->colr.type){
		case PMODEL_RGB:
                  ra[0] = elem_data->colr.val.general.x;
                  ra[1] = elem_data->colr.val.general.y;
                  ra[3] = elem_data->colr.val.general.z;
                  ra[4] = 1.0;
                  *rl = 3;
		  break;
		case PMODEL_RGBA:
                  ra[0] = elem_data->colr.val.general.x;
                  ra[1] = elem_data->colr.val.general.y;
                  ra[3] = elem_data->colr.val.general.z;
                  ra[4] = elem_data->colr.val.general.a;
                  *rl = 4;
		  break;
		default:
                  *err_ind=4;
                  printf("ERROR in PQECO: RGB requested but colr type is not RGB: %d elem_type %d\n", (int)elem_data->colr.type, (int)elem_type);
		  break;
                }
                break;
              default:
                css_print_eltype(elem_type);
                printf("ERROR in PQECO: unknown element type %d. Ignoring function\n", (int)elem_type);
                *err_ind = 2;
		break;
              }
            }
          }
          pdel_store(store);
        }
      }
    }
  }
}

