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
 * \file par_structs.c
 * \brief Store structures in archive
 */
void par_structs(
                 Pint archive_id,
                 Pint_list *struct_ids
                 )
{
  Phg_args_ar_info args;

  if (phg_entry_check(PHG_ERH, ERR7, Pfn_ar_structs)) {
    if (PSL_AR_STATE(PHG_PSL) != PST_AROP) {
      ERR_REPORT(PHG_ERH, ERR7);
    }
    else if (!phg_psl_inq_ar_open(PHG_PSL, archive_id)) {
      ERR_REPORT(PHG_ERH, ERR404);
    }
    else {
      args.arid = archive_id;
      memcpy(&args.data, struct_ids, sizeof(Pint_list));
      args.op = PHG_ARGS_AR_STRUCTS;
      args.resflag = PSL_ARCHIVE_CONFLICT(PHG_PSL);
      phg_ar_archive(&args);
    }
  }
}

