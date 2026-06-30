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
#include <X11/Xlib.h>

#include "phg.h"
#include "private/evtP.h"
#include "private/wsxP.h"

#include <X11/Shell.h>
#include <X11/StringDefs.h>

/*******************************************************************************
 * phg_wsx_input_dispatch_next
 *
 * DESCR:       Process any event on the event queue
 * RETURNS:     TRUE or FALSE
 */

int phg_wsx_input_dispatch_next(
   Ws *ws,
   Phg_sin_evt_tbl *evt_tbl
   )
{
   int status = FALSE;
   XEvent event;

#ifdef GTK4_EXT
   if (g_main_context_iteration(NULL, FALSE)) {
     status = TRUE;
   }
#else
   XtInputMask m;
   XtInputMask t;

   m = XtIMXEvent;
   if (((t = XtAppPending(ws->app_context)) & m)) {
     /* wait for certain events, stepping through choices */
     XtAppPeekEvent(ws->app_context, &event);
     phg_sin_evt_dispatch(evt_tbl, ws->display, &event);
     status = TRUE;
     XtAppProcessEvent(ws->app_context, t & m);
   }
#endif
   if (XCheckWindowEvent(ws->display,
                         ws->input_overlay_window,
                         (unsigned long) 0xffffffffUL,
                         &event) == True) {
      status = TRUE;
   }

   return status;
}
