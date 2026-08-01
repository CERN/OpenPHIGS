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
 * \file pqdlc3.c
 * \brief inquire default locator device data 3
 *
 * \return error index, init position in WC, number of echoes, echoes, echo volume in DC, number of elements in data record, data record
 */
FTN_SUBROUTINE(pqdlc3)(
                       FTN_INTEGER(wtype),
                       FTN_INTEGER(devno),
                       FTN_INTEGER(n),
                       FTN_INTEGER(mldr),
                       int* errind,
                       float* dpx,
                       float* dpy,
                       float* dpz,
                       int* ol,
                       int* pet,
                       float evol[6],
                       int* ldr,
                       char* datrec
                       ) {
  Wst *wst;
  Wst_input_wsdt *idt;

  Pint ws_type = FTN_INTEGER_GET(wtype);
  Pint devnum = FTN_INTEGER_GET(devno);

  *errind = 0;
  if (devnum <= WST_MAX_NUM_LOCATOR_DEVS){
    wst = phg_wst_find(&PHG_WST_LIST, ws_type);
    if (wst == NULL) {
      ERR_REPORT(PHG_ERH, ERR52);
    }
    idt = &wst->desc_tbl.phigs_dt.in_dt;
    *dpx = idt->locators[devnum].position.x;
    *dpy = idt->locators[devnum].position.y;
    *dpz = idt->locators[devnum].position.z;
    evol[0] = idt->locators[devnum].e_volume.x_min;
    evol[1] = idt->locators[devnum].e_volume.x_max;
    evol[2] = idt->locators[devnum].e_volume.y_min;
    evol[3] = idt->locators[devnum].e_volume.y_max;
    evol[4] = idt->locators[devnum].e_volume.z_min;
    evol[5] = idt->locators[devnum].e_volume.z_max;
    *pet =  idt->locators[devnum].pets[0];
  } else {
    *errind = 1;
    ERR_REPORT(PHG_ERH, ERR250);
  }
  *ldr = 0;
}
