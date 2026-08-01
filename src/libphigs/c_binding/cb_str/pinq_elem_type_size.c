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
 * \file pinq_elem_type_size.c
 * \brief Get element type and size
 */
void pinq_elem_type_size(
                         Pint struct_id,
                         Pint elem_num,
                         Pint *err_ind,
                         Pelem_type *elem_type,
                         size_t *elem_size
                         )
{
  Phg_ret ret;

  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR2;
  }
  else if (elem_num < 0) {
    *err_ind = ERR202;
  }
  else {
    ret.err = 0;
    phg_css_inq_el_type_size(PHG_CSS, struct_id, elem_num, &ret);
    if (!ret.err) {
      *elem_type = ret.data.el_type_size.type;
      *elem_size = ret.data.el_type_size.size;
    }
    *err_ind = ret.err;
  }
}

