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
 * \file phg_cb_resize_store.c
 * \brief Resize storage object
 *
 * \return TRUE or FALSE
 */
int phg_cb_resize_store(
                        Pstore store,
                        Pint size,
                        Pint *err_ind
                        )
{
  struct _Pstore old_store;

  *err_ind = 0;
  if (store != NULL) {
    if (store->size < size) {
      old_store.buf = store->buf;
      if (size > 0 && (store->buf = malloc(size)) == NULL) {
        *err_ind = ERR900;
        store->buf = old_store.buf;
      }
      else {
        if (store->size > 0) {
          free(old_store.buf);
        }
        store->size = size;
      }
    }
  }

  return (*err_ind ? FALSE : TRUE);
}

