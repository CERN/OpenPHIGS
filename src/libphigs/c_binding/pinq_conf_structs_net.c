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
#include "private/cbP.h"

/* Automatically generated prototypes */
extern int valid_ar_fname(
                          char *fname
                          );
extern void popen_ar_file(
                   Pint archive_id,
                   char *archive_file
                   );
extern void pclose_ar_file(
                    Pint archive_id
                    );
extern void pset_conf_res(
                   Pconf_res archive_res,
                   Pconf_res retrieval_res
                   );
extern void par_structs(
                 Pint archive_id,
                 Pint_list *struct_ids
                 );
extern void par_struct_nets(
                     Pint archive_id,
                     Pint_list *struct_ids
                     );
extern void par_all_structs(
                     Pint archive_id
                     );
extern void pret_struct_ids(
                     Pint archive_id,
                     Pint num_elems_appl_list,
                     Pint start_ind,
                     Pint_list *ids,
                     Pint *num_elems_impl_list
                     );
extern void pret_structs(
                  Pint archive_id,
                  Pint_list *struct_ids
                  );
extern void pret_struct_nets(
                      Pint archive_id,
                      Pint_list *struct_ids
                      );
extern void pret_all_structs(
                      Pint archive_id
                      );
extern void pdel_structs_ar(
                     Pint archive_id,
                     Pint_list *struct_ids
                     );
extern void pdel_struct_nets_ar(
                         Pint archive_id,
                         Pint_list *struct_ids
                         );
extern void pdel_all_structs_ar(
                         Pint archive_id
                         );
extern void pinq_ar_st(
                Par_st *ar_st
                );
extern void pinq_ar_files(
                   Pstore store,
                   Pint *err_ind,
                   Par_file_list **ar_files
                   );
extern void pinq_conf_res(
                   Pint *err_ind,
                   Pconf_res *archive_res,
                   Pconf_res *retrieval_res
                   );
extern void pinq_all_conf_structs(
                           Pint ar_id,
                           Pint num_elems_appl_list,
                           Pint start_ind,
                           Pint *err_ind,
                           Pint_list *ids,
                           Pint *num_elems_impl_list
                           );
extern void pinq_conf_structs_net(
                           Pint ar_id,
                           Pint struct_id,
                           Pstruct_net_source source,
                           Pint num_elems_appl_list,
                           Pint start_ind,
                           Pint *err_ind,
                           Pint_list *ids,
                           Pint *num_elems_impl_list
                           );


/**
 * \file pinq_conf_structs_net.c
 * \brief Get conflicting structure ids in network
 */
void pinq_conf_structs_net(
                           Pint ar_id,
                           Pint struct_id,
                           Pstruct_net_source source,
                           Pint num_elems_appl_list,
                           Pint start_ind,
                           Pint *err_ind,
                           Pint_list *ids,
                           Pint *num_elems_impl_list
                           )
{
  Phg_args_q_conflicting args;
  Phg_ret ret;

  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR7;
  }
  else if (PSL_AR_STATE(PHG_PSL) != PST_AROP) {
    *err_ind = ERR7;
  }
  else if (!phg_psl_inq_ar_open(PHG_PSL, ar_id)) {
    *err_ind = ERR404;
  }
  else {
    args.op = PHG_ARGS_CONF_NET;
      args.arid = ar_id;
      args.struct_id = struct_id;
      args.src = source;
      ret.err = 0;
      phg_inq_ar_conflicting(&args, &ret);
      if (ret.err) {
        *err_ind = ret.err;
      }
      else {
        *err_ind = 0;
        ids->num_ints = 0;
        *num_elems_impl_list = ret.data.int_list.num_ints;
        if (ret.data.int_list.num_ints > 0) {
          if (start_ind < 0 || start_ind >= ret.data.int_list.num_ints) {
            *err_ind = ERR2201;
          }
          else if (num_elems_appl_list > 0) {
            ids->num_ints = PHG_MIN(num_elems_appl_list,
                                    ret.data.int_list.num_ints - start_ind);
            memcpy(ids->ints, &ret.data.int_list.ints[start_ind],
                   ids->num_ints * sizeof(Pint));
          }
          else if (num_elems_appl_list < 0) {
            *err_ind = ERRN153;
          }
        }
      }
   }
}
