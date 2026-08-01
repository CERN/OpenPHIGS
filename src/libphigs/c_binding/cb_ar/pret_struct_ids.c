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
#include <string.h>

#include "phg.h"
#include "css.h"
#include "ar.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "private/cbP.h"

/**
 * \file pret_struct_ids.c
 * \brief Retreive all structures identifiers for archive
 */
void pret_struct_ids(
                     Pint archive_id,
                     Pint num_elems_appl_list,
                     Pint start_ind,
                     Pint_list *ids,
                     Pint *num_elems_impl_list
                     )
{
  Phg_ret ret;

  if (phg_entry_check(PHG_ERH, ERR7, Pfn_ret_struct_ids)) {
    if (PSL_AR_STATE(PHG_PSL) != PST_AROP) {
      ERR_REPORT(PHG_ERH, ERR7);
    }
    else if (!phg_psl_inq_ar_open(PHG_PSL, archive_id)) {
      ERR_REPORT(PHG_ERH, ERR404);
    }
    else {
      ret.err = 0;
      ids->num_ints = 0;
      *num_elems_impl_list = 0;
      phg_ar_get_names(archive_id, &ret);
      if (ret.err == 0) {
        ids->num_ints = 0;
        *num_elems_impl_list = ret.data.int_list.num_ints;
        if (ret.data.int_list.num_ints > 0) {
          if (start_ind < 0 ||
              start_ind >= ret.data.int_list.num_ints) {
            ERR_REPORT(PHG_ERH, ERR2201);
          }
          else if (num_elems_appl_list > 0) {
            ids->num_ints = PHG_MIN(num_elems_appl_list,
                                    ret.data.int_list.num_ints -
                                    start_ind);
            memcpy(ids->ints, &ret.data.int_list.ints[start_ind],
                   ids->num_ints * sizeof(Pint));
          }
          else if (num_elems_appl_list < 0) {
            ERR_REPORT(PHG_ERH, ERRN153);
          }
        }
      }
      ERR_FLUSH(PHG_ERH);
    }
  }
}

