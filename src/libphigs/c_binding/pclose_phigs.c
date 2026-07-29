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
 * \file pclose_phigs.c
 * \brief Close phigs
 */
void pclose_phigs(
                  void
                  )
{
  if (phg_entry_check(PHG_ERH, ERR4, Pfn_close_phigs)) {
    if ((PSL_WS_STATE(PHG_PSL) == PWS_ST_WSCL) &&
        (PSL_STRUCT_STATE(PHG_PSL) == PSTRUCT_ST_STCL) &&
        (PSL_AR_STATE(PHG_PSL) == PST_ARCL)) {
      free(PHG_WS_LIST);
      free(PHG_INPUT_Q);
      phg_wst_remove_ws_types();
      phg_sin_evt_tbl_destroy(PHG_EVT_TABLE);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      phg_cb_destroy_all_stores();
    }
    else  {
      ERR_REPORT(PHG_ERH, ERR4);
    }
  }
}

