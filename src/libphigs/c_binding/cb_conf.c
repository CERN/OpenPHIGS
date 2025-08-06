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

/*******************************************************************************
 * pxset_conf_file_name
 *
 * DESCR:       set the configuration path and name
 * RETURNS:     N/A
 */
void pxset_conf_file_name(
                          char * name){
  read_config(name);
}

/*******************************************************************************
 * pxset_conf_hcsf
 *
 * DESCR:       set the hardcopy scale factor
 * RETURNS:     N/A
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

/*******************************************************************************
 * pxinq_conf_hcsf
 *
 * DESCR:       inquire the hardcopy scale factor
 * RETURNS:     current hard copy scale factor for given workstation
 */
Pint pxinq_conf_hcsf(
                     Pint wkid
                     ){
  if (wkid >=0 && wkid <100){
    return (Pint)config[wkid].hcsf;
  } else {
    printf("FATAL: configuration error. Work station ID out of range: %d\n", wkid);
    exit(1);
  }
}
