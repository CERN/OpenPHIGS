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

/*******************************************************************************
 * popst
 *
 * DESCR:   Opens a structure for appending or editing.
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(popst)(
                      FTN_INTEGER(strid)
                      )
{
  Pint struct_id = FTN_INTEGER_GET(strid);

  if (phg_entry_check(PHG_ERH, ERR6, Pfn_open_struct)) {
    if (PSL_STRUCT_STATE(PHG_PSL) == PSTRUCT_ST_STCL) {
      if (phg_css_open_struct(PHG_CSS, struct_id) != NULL) {
        PSL_STRUCT_STATE(PHG_PSL) = PSTRUCT_ST_STOP;
        PSL_OPEN_STRUCT(PHG_PSL) = struct_id;
      }
      ERR_FLUSH(PHG_ERH);
    }
    else {
      ERR_REPORT(PHG_ERH, ERR6);
    }
  }
}

/*******************************************************************************
 * pclst
 *
 * DESCR:   Closes a previously opened structure.
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pclst)(
                      void
                      )
{

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_close_struct)) {
    if (PSL_STRUCT_STATE(PHG_PSL) == PSTRUCT_ST_STOP) {
      phg_close_struct(PHG_CSS);
      PSL_STRUCT_STATE(PHG_PSL) = PSTRUCT_ST_STCL;
    }
    else {
      ERR_REPORT(PHG_ERH, ERR5);
    }
  }
}

/*******************************************************************************
 * pdel
 *
 * DESCR:       Delete element
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pdel)(void)
{
#ifdef DEBUG
  printf("DEBUG: delete current element called.\n");
#endif
  pdel_elem();
}

/*******************************************************************************
 * pdst
 *
 * DESCR:       Delete structure
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pdst)(
                     FTN_INTEGER(strid)
                     )
{
  Pint struct_id = FTN_INTEGER_GET(strid);
#ifdef DEBUG
  printf("DEBUG: PDST Delete structre %d called \n", struct_id);
#endif
  pdel_struct(struct_id);
}

/*******************************************************************************
 * posep
 *
 * DESCR:       Offset element pointer
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(posep)(
                      FTN_INTEGER(epo)
                      )
{
  Pint delta = FTN_INTEGER_GET(epo);
#ifdef DEBUG
  printf("DEBUG: POSEP: move element point by %d\n", delta);
#endif
  poffset_elem_ptr(delta);
}

/*******************************************************************************
 * psep
 *
 * DESCR:       Set element pointer
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psep)(
                     FTN_INTEGER(pos)
                     )
{
  Pint elem_ptr_value = FTN_INTEGER_GET(pos);
#ifdef DEBUG
  printf("DEBUG: PSEP: move element point by %d\n", elem_ptr_value);
#endif
  pset_elem_ptr(elem_ptr_value);
}

/*******************************************************************************
 * pseplb
 *
 * DESCR:       Set element pointer at label
 * RETURNS:   N/A
 */

FTN_SUBROUTINE(pseplb)(
                       FTN_INTEGER(label)
                       )
{
  Pint label_id = FTN_INTEGER_GET(label);
#ifdef DEBUG
  printf("DEBUG: PSEPLB: move element to label %d\n", label_id);
#endif
  pset_elem_ptr_label(label_id);
}

/*******************************************************************************
 * psedm
 *
 * DESCR:       Set edit mode
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psedm)(
                      FTN_INTEGER(editmode)
                      )
{
  Pedit_mode emode = (Pedit_mode) FTN_INTEGER_GET(editmode);
#ifdef DEBUG
  printf("DEBUG: PSEDM: set edit mode to %d\n", emode);
#endif
  pset_edit_mode(emode);
}

/*******************************************************************************
 * pqedm
 *
 * DESCR:       Inquire edit mode
 * RETURNS:   error, mode
 */
FTN_SUBROUTINE(pqedm)(
                      Pint *err,
                      Pedit_mode* editmode
                      )
{
#ifdef DEBUG
  printf("DEBUG: PQEDM: edit mode is %d\n", *editmode);
#endif
  pinq_edit_mode(err, editmode);
}

/*******************************************************************************
 * pels
 *
 * DESCR:       element search
 * RETURNS:   error, status, element position
 */
FTN_SUBROUTINE(pels)(
                     FTN_INTEGER(strid),
                     FTN_INTEGER(strtep),
                     FTN_INTEGER(srcdir),
                     FTN_INTEGER(eisn),
                     FTN_INTEGER_ARRAY(eis),
                     FTN_INTEGER(eesn),
                     FTN_INTEGER_ARRAY(ees),
                     Pint *errind,
                     Psearch_status* status,
                     Pint *fndep
                     )
{
  Struct_handle structp;
  Pint struct_id = FTN_INTEGER_GET(strid);
  Pint start_el = FTN_INTEGER_GET(strtep);
  Psearch_dir dir = (Psearch_dir) FTN_INTEGER_GET(srcdir);
  Pelem_type_list incll;
  Pelem_type_list excll;
  incll.num_elem_types = FTN_INTEGER_GET(eisn);
  incll.elem_types = (Pelem_type *)&eis[0];
  excll.num_elem_types = FTN_INTEGER_GET(eesn);
  excll.elem_types = (Pelem_type *)&ees[0];
  if ( !(structp = CSS_STRUCT_EXISTS(PHG_CSS, struct_id)) ) {
    printf("Could not find struct_id %d\n", struct_id);
#ifdef DEBUG
  } else {
    printf("Dumping structure %d\n", struct_id);
      phg_css_print_struct(structp, 0);
#endif
  }
  pelem_search(struct_id, start_el, dir, &incll, &excll, errind, status, fndep);
#ifdef DEBUG
  printf("Search for fndep of type %d gave %d %d pos %d\n",eis[0], *errind, *status, *fndep);
#endif
}

/*******************************************************************************
 * pqstst
 *
 * DESCR:       Inquire structure status
 * RETURNS:   error index, structure status index
 */
FTN_SUBROUTINE(pqstst)(
                       FTN_INTEGER(strid),
                       Pint* err_ind,
                       Pstruct_status* strsti
                       ) {
  Pint struct_id = FTN_INTEGER_GET(strid);
  Pint struct_elem;
  pinq_struct_status(struct_id, err_ind, strsti);
}

/*******************************************************************************
 * pqeco
 *
 * DESCR:       Inquire element content
 * RETURNS:   error indicator, num ints, ints array, num float, float array,
 *              num strings, array of string lengths, character string entries
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

/*******************************************************************************
 * dumpstr
 *
 * DESCR:       Print structure as string
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(dumpstr)(FTN_INTEGER(strid)){
  Struct_handle structp;
  Pint struct_id = FTN_INTEGER_GET(strid);
  if ( !(structp = CSS_STRUCT_EXISTS(PHG_CSS, struct_id)) ) {
    printf("Could not find struct_id %d\n", struct_id);
  } else {
    printf("Dumping structure %d\n", struct_id);
    phg_css_print_struct(structp, 0);
  }
}

/*******************************************************************************
 * pcelst
 *
 * DESCR:       Copy all elements from structure
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pcelst)(FTN_INTEGER(strid)){
  Pint struct_id = FTN_INTEGER_GET(strid);
  pcopy_all_elems_struct(struct_id);
}

/*******************************************************************************
 * pdas
 *
 * DESCR:       Delete all structures
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pdas)(){
  pdel_all_structs();
}

/*******************************************************************************
 * pqstrs
 *
 * DESCR:       Inquire structure state value
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pqstrs)(Pint* strsta){
  pinq_struct_st(strsta);
}
