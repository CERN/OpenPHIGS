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
#include <string.h>
#include <sys/types.h>

#include "phg.h"
#include "private/phgP.h"
#include "css.h"
#include "ws.h"
#include "ws_type.h"
#include "private/wsxP.h"
#include "private/evtP.h"
#include "private/cbP.h"

/* Automatically generated prototypes */
extern void popen_phigs(
                 char *error_file,
                 size_t memory
                 );
extern void pclose_phigs(
                  void
                  );
extern void pcreate_store(
                   Pint *err_ind,
                   Pstore *store
                   );
extern void pdel_store(
                Pstore store
                );


/**
 * \file pdel_store.c
 * \brief Delete storage object
 */
void pdel_store(
                Pstore store
                )
{
  Pstore *node;

  for (node = &phg_cb_store_list; *node != NULL; node = &(*node)->next) {
    if (*node == store) {
      *node = (*node)->next;
      if (store->size > 0) {
        free(store->buf);
      }
      free(store);
      break;
    }
  }
}
