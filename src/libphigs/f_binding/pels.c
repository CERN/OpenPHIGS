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
#include "css.h"
#include "private/phgP.h"
#include "private/cbP.h"
#include "util/ftn.h"

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(popst)(;
extern FTN_SUBROUTINE(pclst)(;
extern FTN_SUBROUTINE(pdel)(void);
extern FTN_SUBROUTINE(pdst)(;
extern FTN_SUBROUTINE(posep)(;
extern FTN_SUBROUTINE(psep)(;
extern FTN_SUBROUTINE(pseplb)(;
extern FTN_SUBROUTINE(psedm)(;
extern FTN_SUBROUTINE(pqedm)(;
extern FTN_SUBROUTINE(pels)(;
extern FTN_SUBROUTINE(pqstst)(;
extern FTN_SUBROUTINE(pqeco)(;
extern FTN_SUBROUTINE(dumpstr)(FTN_INTEGER(strid));
extern FTN_SUBROUTINE(pcelst)(FTN_INTEGER(strid));
extern FTN_SUBROUTINE(pdas)();
extern FTN_SUBROUTINE(pqstrs)(Pint* strsta);


/**
 * \file pels.c
 * \brief element search
 *
 * \return error, status, element position
 */
FTN_SUBROUTINE(pels)(
                     FTN_INTEGER(strid),
                     FTN_INTEGER(strtep),
                     FTN_INTEGER(srcdir),
                     FTN_INTEGER(eisn),
                     FTN_INTEGER_ARRAY(eis),
                     FTN_INTEGER(eesn),
                     FTN_INTEGER_ARRAY(ees),
                     Pint *errind,
                     Psearch_status* status,
                     Pint *fndep
                     )
{
  Struct_handle structp;
  Pint struct_id = FTN_INTEGER_GET(strid);
  Pint start_el = FTN_INTEGER_GET(strtep);
  Psearch_dir dir = (Psearch_dir) FTN_INTEGER_GET(srcdir);
  Pelem_type_list incll;
  Pelem_type_list excll;
  incll.num_elem_types = FTN_INTEGER_GET(eisn);
  incll.elem_types = (Pelem_type *)&eis[0];
  excll.num_elem_types = FTN_INTEGER_GET(eesn);
  excll.elem_types = (Pelem_type *)&ees[0];
  if ( !(structp = CSS_STRUCT_EXISTS(PHG_CSS, struct_id)) ) {
    printf("Could not find struct_id %d\n", struct_id);
#ifdef DEBUG
  } else {
    printf("Dumping structure %d\n", struct_id);
      phg_css_print_struct(structp, 0);
#endif
  }
  pelem_search(struct_id, start_el, dir, &incll, &excll, errind, status, fndep);
#ifdef DEBUG
  printf("Search for fndep of type %d gave %d %d pos %d\n",eis[0], *errind, *status, *fndep);
#endif
}

