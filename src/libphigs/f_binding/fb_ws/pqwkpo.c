/******************************************************************************
 *   Do NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
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
#include <math.h>

#include <gl2ps.h>
#include "phigs.h"
#include "phg.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "private/wsglP.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"

extern int record_geom;

/**
 * \file pqwkpo.c
 * \brief Inquire set member of workstations to which posted
 *
 * \return error index, number of workstations to which the structure is posted, Nth member of set of workstations to which the structure is posted
 */

FTN_SUBROUTINE(pqwkpo)(
                       FTN_INTEGER(strid),
                       FTN_INTEGER(n),
                       FTN_INTEGER(err_ind),
                       FTN_INTEGER(ol),
                       FTN_INTEGER(wkid)
                       )
{
  /*
    This needs to loop over all work stations and all their posted structures, and if the given structure is found,
    remember that WKID and the number of matches we had. Then we return the WKID of the N'th match in wkid and the number of matches in ol
    FIXME: this one does not seem to find anything for some reason.

  */
  Pint struct_id = FTN_INTEGER_GET(strid);
  Ws_handle wsh;
  Pint num = FTN_INTEGER_GET(n);
  Pint matches, nwk, ws_id;
  Pint wkids[99];
  Ws_post_str * current;
  Wsb_output_ws *ows;
  Ws_posted_structs posted;
#ifdef DEBUG
  printf("DEBUG: PQWKPO inquire work station \n");
#endif
  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR2;
  } else {
    nwk = PHG_WST_LIST.count;
    matches = 0;
    *err_ind = 1;
    for (ws_id = 0; ws_id<nwk; ws_id++){
      wsh = PHG_WSID(ws_id);
      if (wsh != NULL){
        ows = &wsh->out_ws.model.b;
        posted = ows->posted;
        current =  &posted.highest;
        while (current != NULL) {
          if (current->structh != NULL) {
            if (current->structh->struct_id == struct_id) {
              wkids[matches] = ws_id;
              matches += 1;
            }
          }
          current = current->lower;
        }
      }
    }
    if (matches>0 && num<=matches){
      *err_ind = 0;
      *ol = matches;
      *wkid = wkids[num-1];
#ifdef DEBUG
      printf("PQWKPO: Found %d matches. Returning %d\n", *ol, *wkid);
#endif
    } else {
      *err_ind = ERR201;
      *ol = 0;
      *wkid = 0;
#ifdef DEBUG
      printf("PQWKPO: No matches found.\n");
#endif
    }
  }
}

