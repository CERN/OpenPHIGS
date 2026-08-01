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
 * \file pget_pick.c
 * \brief Get pick event from event queue
 */
void pget_pick(
               Pint depth,
               Pin_status *in_status,
               Ppick_path *pick
               )
{
  Ppick *pik;
  Pint depth_limit;

  if (PSL_CUR_EVENT_CLASS(PHG_PSL) != PIN_NONE){
    if (check_event_class(PIN_PICK, Pfn_get_pick)) {
      pik = &PSL_CUR_EVENT_DATA(PHG_PSL, pik);
      *in_status = pik->status;
      if (pik->status == PIN_STATUS_OK) {
        pick->depth = pik->pick_path.depth;
#ifdef DEBUGINP
        printf("Pick status is OK. Depth: %d\n", pick->depth);
#endif
        depth_limit = PHG_MIN(depth, pik->pick_path.depth);
        if (depth_limit > 0) {
#ifdef DEBUGINP
          printf("depth limit is %d", depth_limit);
#endif
          memcpy(pick->path_list,
                 pik->pick_path.path_list,
                 depth_limit * sizeof(Ppick_path_elem));
        }
      }
#ifdef DEBUGINP
      else {
        printf("Pick status is not OK: %d\n", pik->status);
      }
#endif
    }
  } else {
#ifdef DEBUGINP
    printf("No input");
#endif
    *in_status = PIN_STATUS_NO_IN;
  }
}

