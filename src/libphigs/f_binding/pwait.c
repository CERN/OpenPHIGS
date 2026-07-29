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
 * \file pwait.c
 * \brief await event
 *
 * \return work station, class, number
 */
FTN_SUBROUTINE(pwait)(
                      FTN_REAL(tout),
                      Pint *wkid,
                      Pin_class *icl,
                      Pint *idnr
                      )
{
  Pfloat timeout = FTN_REAL_GET(tout);
  pawait_event(timeout, wkid, icl, idnr);
}

