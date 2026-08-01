/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2022-2023 CERN
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <private/wsxP.h>
#include <util/ftn.h>

/**
 * \file pspkft.c
 * \brief set pick filter
 */
FTN_SUBROUTINE(pspkft)(
                       FTN_INTEGER(wkid),
                       FTN_INTEGER(pkdnr),
                       FTN_INTEGER(isn),
                       Pint *is,
                       FTN_INTEGER(esn),
                       Pint *es
                       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint pick_dev =  FTN_INTEGER_GET(pkdnr);
  Pint isn1 = FTN_INTEGER_GET(isn);
  Pint esn1 = FTN_INTEGER_GET(esn);

  Pfilter filter;

  Pint incl_set[isn1+1];
  Pint excl_set[esn1+1];
  memcpy(&incl_set, is, isn1*sizeof(int));
  memcpy(&excl_set, es, esn1*sizeof(int));

  filter.incl_set.num_ints = isn1;
  filter.excl_set.num_ints = esn1;
  if (isn == 0) incl_set[0] = 0;
  if (esn == 0) excl_set[0] = 0;
  filter.incl_set.ints = &incl_set[0];
  filter.excl_set.ints = &excl_set[0];

  pset_pick_filter(ws_id, pick_dev, &filter);
}

