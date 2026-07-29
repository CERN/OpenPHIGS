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
 * \file pinq_conf_res.c
 * \brief Get archive conflict resolution
 */
void pinq_conf_res(
                   Pint *err_ind,
                   Pconf_res *archive_res,
                   Pconf_res *retrieval_res
                   )
{
  if (phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = 0;
    *archive_res = PSL_ARCHIVE_CONFLICT(PHG_PSL);
    *retrieval_res = PSL_RETRIEVE_CONFLICT(PHG_PSL);
  }
  else {
    *err_ind = ERR2;
  }
}

