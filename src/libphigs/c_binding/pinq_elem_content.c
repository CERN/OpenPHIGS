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
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cbP.h"

/* Automatically generated prototypes */
extern void popen_struct(
                  Pint struct_id
                  );
extern void pclose_struct(
                   void
                   );
extern void pchange_struct_id(
                       Pint orig_struct_id,
                       Pint result_struct_id
                       );
extern void pchange_struct_refs(
                         Pint orig_struct_id,
                         Pint result_struct_id
                         );
extern void pchange_struct_id_refs(
                            Pint orig_struct_id,
                            Pint result_struct_id
                            );
extern void pset_edit_mode(
                    Pedit_mode edit_mode
                    );
extern void pset_elem_ptr(
                   Pint elem_ptr_value
                   );
extern void poffset_elem_ptr(
                      Pint elem_ptr_offset
                      );
extern void pset_elem_ptr_label(
                         Pint label_id
                         );
extern void pdel_elem(
               void
               );
extern void pdel_elem_range(
                     Pint elem_ptr1_value,
                     Pint elem_ptr2_value
                     );
extern void pdel_elems_labels(
                       Pint label1_id,
                       Pint label2_id
                       );
extern void pempty_struct(
                   Pint struct_id
                   );
extern void pdel_struct(
                 Pint struct_id
                 );
extern void pdel_struct_net(
                     Pint struct_id,
                     Pref_flag ref_flag
                     );
extern void pdel_all_structs(
                      void
                      );
extern void pcopy_all_elems_struct(
                            Pint struct_id
                            );
extern void pelem_search(
                  Pint struct_id,
                  Pint struct_elem,
                  Psearch_dir dir,
                  Pelem_type_list *incl,
                  Pelem_type_list *excl,
                  Pint *err_ind,
                  Psearch_status *status,
                  Pint *found_elem_ptr
                  );
extern void pinq_edit_mode(
                    Pint *err_ind,
                    Pedit_mode *edit_mode
                    );
extern void pinq_elem_ptr(
                   Pint *err_ind,
                   Pint *elem_ptr_value
                   );
extern void pinq_struct_st(
                    Pint *struct_st
                    );
extern void pinq_open_struct(
                      Pint *err_ind,
                      Popen_struct_status *status,
                      Pint *struct_id
                      );
extern void pinq_struct_status(
                        Pint struct_id,
                        Pint *err_ind,
                        Pstruct_status *status
                        );
extern void pinq_elem_type_size(
                         Pint struct_id,
                         Pint elem_num,
                         Pint *err_ind,
                         Pelem_type *elem_type,
                         size_t *elem_size
                         );
extern void pinq_cur_elem_type_size(
                             Pint *err_ind,
                             Pelem_type *elem_type,
                             size_t *elem_size
                             );
extern void pinq_elem_content(
                       Pint struct_id,
                       Pint elem_num,
                       Pstore store,
                       Pint *err_ind,
                       Pelem_data **elem_data
                       );
extern void pinq_cur_elem_content(
                           Pstore store,
                           Pint *err_ind,
                           Pelem_data **elem_data
                           );


/**
 * \file pinq_elem_content.c
 * \brief Get element content
 */
void pinq_elem_content(
                       Pint struct_id,
                       Pint elem_num,
                       Pstore store,
                       Pint *err_ind,
                       Pelem_data **elem_data
                       )
{
  Phg_ret ret;
  Phg_elmt_info *el_info;
  int size;

  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR2;
  }
  else if (elem_num == 0) {
    *err_ind = 0;
    *elem_data = NULL;
  }
  else if (elem_num < 0) {
    *err_ind = ERR202;
  }
  else {
    ret.err = 0;
    phg_css_inq_el_content(PHG_CSS, struct_id, elem_num, &ret);
    if (!ret.err) {
      if (ret.data.el_info.op != PELEM_NIL) {
        el_info = ret.data.el_info.el_head;
        size = phg_cb_store_el_size(el_info);
        if (phg_cb_resize_store(store, size, err_ind)) {
          phg_cb_store_el_data(el_info, store->buf,
                               &store->data.elem_data);
          *elem_data = &((struct _Pstore *) store)->data.elem_data;
        }
      }
    }
    *err_ind = ret.err;
  }
}

