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
*******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "util/ftn.h"
#include "phconf.h"

/**
 * \file pxoirm.c
 *
 * \brief   Set the Order Independent Rendering mode for workstation (OpenPHIGS extension)
 *
 * \param   wkid work station ID
 * \param   OIR mode
 *
 * Possible modes:
 *  - 0: Switch OFF Order Independent Rendering
 *  - 1: Use Default mode.
 *  - 2: Alternative way of blending transparent surfaces.
 *
 * \note This setting is only relevant if shader version 430 is in use. This setting can be set via the configuration as well. As it is used to configure the workstation, the workstation must not be open yet in order to have an effect.
 *
 * \pre The function must be called BEFORE the workstation is opened.
 *
 * \sa popwk
 */
FTN_SUBROUTINE(pxoirm)(
                        FTN_INTEGER(wkid),
                        FTN_INTEGER(mode)
                       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint oirmode = FTN_REAL_GET(mode);
  pxset_oir_mode(ws_id, oirmode);
}
