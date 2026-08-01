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
 * \file pelem_search.c
 * \brief Get all matching elements
 */
void pelem_search(
                  Pint struct_id,
                  Pint struct_elem,
                  Psearch_dir dir,
                  Pelem_type_list *incl,
                  Pelem_type_list *excl,
                  Pint *err_ind,
                  Psearch_status *status,
                  Pint *found_elem_ptr
                  )
{
  Phg_ret ret;

  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR2;
  }
  else {
    ret.err = 0;
    phg_css_el_search(PHG_CSS, struct_id, struct_elem, dir, incl, excl, &ret);
    if (ret.err) {
      *err_ind = ret.err;
    }
    else {
      *err_ind = 0;
      *status = ret.data.el_search.status;
      *found_elem_ptr = ret.data.el_search.found_el;
    }
  }
}

