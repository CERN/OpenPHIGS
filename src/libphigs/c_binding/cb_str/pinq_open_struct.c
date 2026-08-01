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
#include "private/cb_internal.h"
#include "private/cbP.h"

/**
 * \file pinq_open_struct.c
 * \brief Get current structure open and edit status
 */
void pinq_open_struct(
                      Pint *err_ind,
                      Popen_struct_status *status,
                      Pint *struct_id
                      )
{
  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR2;
  }
  else if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
    *status = PSTRUCT_NONE;
      *err_ind = 0;
  }
  else {
    *err_ind = 0;
    *status = PSTRUCT_OPEN;
    *struct_id = CSS_CUR_STRUCT_ID(PHG_CSS);
  }
}

