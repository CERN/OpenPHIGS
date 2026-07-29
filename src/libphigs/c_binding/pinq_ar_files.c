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
 * \file pinq_ar_files.c
 * \brief Get open archive files
 */
void pinq_ar_files(
                   Pstore store,
                   Pint *err_ind,
                   Par_file_list **ar_files
                   )
{
  int i, j, size;
  char *name_buf;

  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR2;
  }
  else {
    *err_ind = 0;
    *ar_files = &((struct _Pstore *) store)->data.ar_files;
    (*ar_files)->num_ar_files = 0;
    if (PSL_AR_STATE(PHG_PSL) == PST_AROP) {
      for (i = 0, size = 0; i < MAX_NO_OPEN_ARFILES; i++) {
        if (PHG_PSL->ar_files[i].used) {
          (*ar_files)->num_ar_files++;
          size += strlen(PHG_PSL->ar_files[i].fname) + 1;
        }
        size += (*ar_files)->num_ar_files * sizeof(Par_file);
        if (phg_cb_resize_store(store, size, err_ind)) {
          j = 0;
          (*ar_files)->ar_files =
            (Par_file *) ((struct _Pstore *) store)->buf;
          name_buf = (char *)
            ((*ar_files)->ar_files + (*ar_files)->num_ar_files);
          for (i = 0; i < MAX_NO_OPEN_ARFILES; i++) {
            if (PHG_PSL->ar_files[i].used) {
              (*ar_files)->ar_files[j].id = PHG_PSL->ar_files[i].arid;
              (*ar_files)->ar_files[j].name = name_buf;
              strcpy(name_buf, PHG_PSL->ar_files[i].fname);
              name_buf += strlen(PHG_PSL->ar_files[i].fname) + 1;
              j++;
            }
          }
        }
      }
    }
  }
}

