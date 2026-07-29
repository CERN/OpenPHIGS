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

#include "phg.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "private/sinqP.h"
#include "private/wsxP.h"

/**
 * \file input_ws_open.c
 * \brief Open input workstation desciption helper function
 *
 * \return Pointer to input description or NULL
 */
Wst_input_wsdt* input_ws_open(
                                     Pint ws_id,
                                     Pint fn_id,
                                     Wst_phigs_dt **dtp,
                                     Pint *err_ind
                                     )
{
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Wst_input_wsdt *idt = NULL;

  if (err_ind == NULL) {
    ERR_SET_CUR_FUNC(PHG_ERH, fn_id);
  }

  if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    if (err_ind == NULL) {
      ERR_REPORT(PHG_ERH, ERR3);
    }
    else {
      *err_ind = ERR3;
    }
  }
  else {
    wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
    if (wsinfo == NULL) {
      if (err_ind == NULL) {
        ERR_REPORT(PHG_ERH, ERR54);
      }
      else {
        *err_ind = ERR54;
      }
    }
    else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!((dt->ws_category == PCAT_OUTIN) ||
            (dt->ws_category == PCAT_IN))) {
        if (err_ind == NULL) {
          ERR_REPORT(PHG_ERH, ERR61);
        }
        else {
          *err_ind = ERR61;
        }
      }
      else {
        idt = &wsinfo->wstype->desc_tbl.phigs_dt.in_dt;
        if (dtp != NULL) {
          *dtp = dt;
        }
      }
    }
  }

  return idt;
}

