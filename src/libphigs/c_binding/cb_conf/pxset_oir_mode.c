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

/**
 * \file pxset_oir_mode.c
 *
 * \brief   set the Out of Order rendering mode. 0=disabled, >0 number of layers per pixel
 *
 * \param   ws_id workstation ID
 * \param   mode
 *
 * Possible modes:
 *  - 0: Switch OFF Order Independent Rendering
 *  - 1: Use Default mode.
 *  - 2: Alternative way of blending transparent surfaces.
 *
 * \note This setting is only relevant if shader version 430 is in use. This setting can be set via the configuration as well. As it is used to configure the workstation, the workstation must not be open yet in order to have an effect.
 *
 * \pre This setting can be set via the configuration as well. As it is used to configure the workstation, the workstation must not be open yet in order to have an effect.
 *.
 * \sa popen_wk
 */
#include "phg.h"
#include "css.h"
#include "ws.h"
#include "private/phgP.h"
void pxset_oir_mode(
                     Pint ws_id,
                     Pint mode
                     ){
  Ws_handle wsh;
  if (phg_ws_open(ws_id, Pfn_close_ws) != NULL) {
    /* workstation is already open. Ignoring the call.*/
    ERR_REPORT(PHG_ERH, ERR53);
    return;
  }
  if (ws_id >=0 && ws_id <100){
    if (mode >=0 && mode <=16) {
      switch (mode) {
      case 0:
        config[ws_id].oir = 0;
        config[ws_id].layersPerPixel = 0;
        break;
      default:
        config[ws_id].oir = 1;
        config[ws_id].layersPerPixel = mode;
        break;
      }
    } else {
      printf("ERROR: configuration error. OIR mode is out of range. Got %d\n", mode);
      return;
    }
  } else {
    printf("FATAL: configuration error. Work station ID out of range: %d\n", ws_id);
    exit(1);
  }
}
