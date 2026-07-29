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

#include <stdlib.h>
#include <stdio.h>

#include "phg.h"
#include "private/phgP.h"
#include "private/cbP.h"

extern struct _Pstore *phg_cb_store_list;

/* Automatically generated prototypes */
extern int phg_cb_resize_store(
                        Pstore store,
                        Pint size,
                        Pint *err_ind
                        );
extern void phg_cb_destroy_all_stores(
                               void
                               );
extern int phg_cb_store_el_size(
                         Phg_elmt_info *el_info
                         );
extern void phg_cb_store_el_data(
                          Phg_elmt_info *el_info,
                          void *buf,
                          Pelem_data *ed
                          );


/**
 * \file phg_cb_store_el_size.c
 * \brief Get size for store data buffer used by inquiry function
 *
 * \return Element size
 */
int phg_cb_store_el_size(
                         Phg_elmt_info *el_info
                         )
{
  Pint *idata;
  int size;

  switch(el_info->elementType) {
  case PELEM_LABEL:
    idata = (Pint *) &el_info[1];
    size = sizeof(Pint);

  case PELEM_FILL_AREA_SET:
    idata = (Pint *) &el_info[1];
    size = sizeof(Ppoint_list) * (*idata);
    break;

  case PELEM_FILL_AREA_SET3:
    idata = (Pint *) &el_info[1];
    size = sizeof(Ppoint_list3) * (*idata);
    break;

  case PELEM_FILL_AREA_SET_DATA:
    /* TODO */
    size = 0;
    printf("TODO: phg_cb_store_el_size element type %d, length %d\n", el_info->elementType, el_info->length);
    break;

  case PELEM_FILL_AREA_SET3_DATA:
    /* TODO */
    printf("TODO: phg_cb_store_el_size element type %d, length %d\n", el_info->elementType, el_info->length);
    size = 0;
    break;

  case PELEM_SET_OF_FILL_AREA_SET3_DATA:
    printf("TODO: phg_cb_store_el_size element type %d, length %d\n", el_info->elementType, el_info->length);
    /* TODO */
    size = 0;
    break;
  case PELEM_INT_COLR_IND:
  case PELEM_EDGE_COLR_IND:
  case PELEM_MARKER_COLR_IND:
  case PELEM_TEXT_COLR_IND:
    idata = (Pint *) &el_info[1];
    size = sizeof(Pint) * (*idata);
    break;
  default:
    size = 0;
    break;
  }

  return (size);
}

