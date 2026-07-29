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
 * \file set_mode.c
 * \brief Set mode helper function
 */
void set_mode(
                     Pint ws_id,
                     Phg_args_idev_class dev_class,
                     Pint dev_num,
                     Pop_mode op_mode,
                     Pecho_switch echo_switch
                     )
{
  Phg_args_set_mode_data args;

  /* The calling function shall always check the requested workstation first */
  Ws_handle wsh = PHG_WSID(ws_id);

  args.idev_class = dev_class;
  args.dev = dev_num;
  args.mode = op_mode;
  args.echo = echo_switch;

  (*wsh->set_device_mode)(wsh, &args);
}

