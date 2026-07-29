/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*             (C) 2022-2023 CERN
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
*******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "phconf.h"

/* Automatically generated prototypes */
extern void pxset_conf_file_name(
                          char * name);
extern void pxset_conf_hcsf(
                     Pint wkid,
                     Pfloat hcsf
                     );
extern Pint pxinq_conf_hcsf(
                     Pint wkid
                     );


/**
 * \file pxset_conf_hcsf.c
 * \brief set the hardcopy scale factor
 */
void pxset_conf_hcsf(
                     Pint wkid,
                     Pfloat hcsf
                     ){
  if (wkid >=0 && wkid <100){
    if (hcsf > 0. && hcsf <= 32){
      config[wkid].hcsf = hcsf;
    } else {
      printf("ERROR: configuration error. Ignoring unreasonable scale factor of: %f\n", hcsf);
    }
  } else {
    printf("FATAL: configuration error. Work station ID out of range: %d\n", wkid);
    exit(1);
  }
}

