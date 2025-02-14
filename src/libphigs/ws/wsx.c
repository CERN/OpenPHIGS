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
#include <string.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xlib.h>

#include "phg.h"
#include "ws.h"
#include "cp.h"
#include "private/wsglP.h"
#include "private/wsxP.h"

/*******************************************************************************
 * phg_wsx_create
 *
 * DESCR:       Create workstation
 * RETURNS:     Pointer to workstaion of NULL
 */

Ws* phg_wsx_create(
   Phg_args_open_ws *args
   )
{
   Ws *ws;

   ws = (Ws *) calloc(1, sizeof(Ws));
   if (ws == NULL) {
      ERR_BUF(args->erh, ERR900);
   }
   else if (!PHG_SCRATCH_SPACE(&ws->scratch, args->memory)) {
      ERR_BUF(args->erh, ERR900);
      free(ws);
      ws = NULL;
   }
   else {
      ws->erh  = args->erh;
      ws->id   = args->wsid;
      ws->type = args->type;
   }
   ws->display = NULL;
   ws->drawable_id = 0;
   /* init widgets */
   ws->top_level = NULL;
   ws->shell = NULL;
   ws->msg_shell = NULL;
   ws->msg_label = NULL;
   ws->valuator_shell = NULL;
   ws->valuator_box = NULL;
   ws->valuator_frame = NULL;
   ws->num_boxed_valuators = 0;
   return ws;
}
/*******************************************************************************
 * Error handler
 ******************************************************************************/
int X11ErrorHandler(Display * display, XErrorEvent * error)
{
  printf("An X11 error was detected: code: %d", error->error_code);
  return 0;
}
/*******************************************************************************
 * phg_wsx_setup_tool
 *
 * DESCR:       Create window
 * RETURNS:     TRUE or FALSE
 */

int phg_wsx_setup_tool(
		       Ws *ws,
		       Phg_args_conn_info *conn_info,
		       Wst *wst
		       )
{
  Pint err_ind;
  XVisualInfo *best_info;
  Colormap cmap;
  Pgcolr background;
  XSetWindowAttributes attrs;
  Window drawable_id;
  XSizeHints size_hints;
  XEvent event;
  int status = FALSE;
  Wst_xwin_dt *xdt = &wst->desc_tbl.xwin_dt;
  Display *display = ws->display;
  int         argc = 0;
  char        **argv = (char **)NULL;

  /* Find matching visual */
  phg_wsx_find_best_visual(ws, wst, &best_info, &cmap, &err_ind);
  if (err_ind != 0) {
    ERR_BUF(ws->erh, err_ind);
    status = FALSE;
  }
  else {
    /* Initial attributes */
    XSetErrorHandler(X11ErrorHandler);
    attrs.colormap = cmap;
    attrs.border_pixel = WhitePixel(display, best_info->screen);
    attrs.background_pixel = BlackPixel(display, best_info->screen);

    ws->glx_context = phg_wsx_create_context(ws, best_info, &err_ind);
    if (err_ind != 0) {
      ERR_REPORT(ws->erh, err_ind);
      status = FALSE;
    }
    else {
      /* Initialize rendering context */
      ws->app_context = phg_cpm_init_toolkit(argc, argv);
      ws->top_level = XtInitialize("Workstation", "", NULL, 0, &argc, argv);
      /* Create window */
      drawable_id = XCreateWindow(display,
				  RootWindow(display, best_info->screen),
				  xdt->tool.x, xdt->tool.y,
				  xdt->tool.width, xdt->tool.height,
				  xdt->tool.border_width, best_info->depth,
				  InputOutput, best_info->visual,
				  CWColormap | CWBackPixel | CWBorderPixel,
				  &attrs);
      if (!drawable_id) {
	ERR_BUF(ws->erh, ERRN203);
	status = FALSE;
      }
      else {
	/* Initialize attributes */
	size_hints.flags = USPosition | USSize;
	size_hints.x = xdt->tool.x;
	size_hints.y = xdt->tool.y;
	size_hints.width = xdt->tool.width;
	size_hints.height = xdt->tool.height;
	XSetStandardProperties(display, drawable_id, xdt->tool.label,
			       xdt->tool.icon_label, None, NULL, 0,
			       &size_hints);
	XSelectInput(display, drawable_id, (long) ExposureMask);
	XMapWindow(display, drawable_id);
	XSync(display, False);

	XWindowEvent(display, drawable_id, ExposureMask, &event);
	XSelectInput(display, drawable_id, (long) 0);
	ws->drawable_id = drawable_id;
	/* Initialize renderer */
	phg_wsx_pixel_colour(ws, cmap, attrs.background_pixel, &background);
	if (!wsgl_init(ws, &background, NUM_SELECTABLE_STRUCTS)) {
	  ERR_BUF(ws->erh, ERR900);
	  free(ws);
	  status = FALSE;
	}
	else {
	  status = TRUE;
	}
      }
    }
  }
  return status;
}

/*******************************************************************************
 * phg_wsx_release_window
 *
 * DESCR:       Release window
 * RETURNS:     N/A
 */

void phg_wsx_release_window(
   Ws *ws
   )
{
   wsgl_close(ws);
   XDestroyWindow(ws->display, ws->drawable_id);
}

/*******************************************************************************
 * phg_wsx_destroy
 *
 * DESCR:       Destroy workstation
 * RETURNS:     N/A
 */

void phg_wsx_destroy(
   Ws *ws
   )
{
   free(ws);
}
