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

/*******************************************************************************
 * popen_phigs
 *
 * DESCR:       Open phigs
 * RETURNS:     N/A
 */

void popen_phigs(
   char *error_file,
   size_t memory
   )
{
   phg = (Phg_handle) malloc(sizeof(Phg_struct));
   if (phg == NULL) {
      goto abort;
   }
   memset(phg, 0, sizeof(Phg_struct));

   PHG_ERH = phg_erh_create(error_file);
   if (PHG_ERH == NULL) {
      goto abort;
   }

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_open_phigs);

   PHG_PSL = phg_psl_create();
   if (PHG_PSL == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      goto abort;
   }

   PHG_CSS = phg_css_init(PHG_ERH, SSH_CSS);
   if (PHG_CSS == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   PHG_EVT_TABLE = phg_sin_evt_tbl_create(PHG_NUM_EVENTS);
   if (PHG_EVT_TABLE == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   PHG_INPUT_Q = phg_sin_q_create(PHG_ERH);
   if (PHG_INPUT_Q == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      phg_sin_evt_tbl_destroy(PHG_EVT_TABLE);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   /* At least one output workstation */
   list_init(&PHG_WST_LIST);
   if (!phg_wst_add_ws_type(PCAT_OUT, 0)) {
      ERR_REPORT(PHG_ERH, ERR900);
      free(PHG_INPUT_Q);
      phg_sin_evt_tbl_destroy(PHG_EVT_TABLE);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }

   /* Optional workstation types */
   phg_wst_add_ws_type(PCAT_OUT, 1);
   phg_wst_add_ws_type(PCAT_OUTIN, 0);
   phg_wst_add_ws_type(PCAT_OUTIN, 1);
   phg_wst_add_ws_type(PCAT_TGA, 0);
   phg_wst_add_ws_type(PCAT_PNG, 0);
   phg_wst_add_ws_type(PCAT_PNGA, 0);
   phg_wst_add_ws_type(PCAT_EPS, 0);
   phg_wst_add_ws_type(PCAT_PDF, 0);
   phg_wst_add_ws_type(PCAT_SVG, 0);

   PHG_WS_LIST = (Ws_handle *) malloc(sizeof(Ws_handle) * MAX_NO_OPEN_WS);
   if (PHG_WS_LIST == NULL) {
      ERR_REPORT(PHG_ERH, ERR900);
      free(PHG_INPUT_Q);
      phg_wst_remove_ws_types();
      phg_sin_evt_tbl_destroy(PHG_EVT_TABLE);
      phg_css_destroy(PHG_CSS);
      phg_psl_destroy(PHG_PSL);
      goto abort;
   }
   memset(PHG_WS_LIST, 0, sizeof(Ws_handle) * MAX_NO_OPEN_WS);

   PSL_SYS_STATE(PHG_PSL) = PSYS_ST_PHOP;

abort:
   ERR_FLUSH(PHG_ERH);
}

/*******************************************************************************
 * pclose_phigs
 *
 * DESCR:       Close phigs
 * RETURNS:     N/A
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

/*******************************************************************************
 * pcreate_store
 *
 * DESCR:       Create storage object
 * RETURNS:     N/A
 */

void pcreate_store(
   Pint *err_ind,
   Pstore *store
   )
{
   *store = (Pstore) calloc(1, sizeof(struct _Pstore));
   if (*store == NULL) {
      *err_ind = ERR900;
   }
   else {
      *err_ind = 0;
      (*store)->next = phg_cb_store_list;
      phg_cb_store_list = *store;
   }
}

/*******************************************************************************
 * pdel_store
 *
 * DESCR:       Delete storage object
 * RETURNS:     N/A
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
