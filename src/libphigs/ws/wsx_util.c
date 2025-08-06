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
#ifdef GLEW
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>

#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "private/wsglP.h"

/*******************************************************************************
 * get_sharable_colormap
 *
 * DESCR:	Get sharable colormap
 * RETURNS:	Colormap
 */

static Colormap get_sharable_colormap(
   XVisualInfo *vi,
   Display *dpy
   )
{
  Status status;
  XStandardColormap *standardCmaps;
  Colormap cmap;
  int i, numCmaps;

  /* If no standard colormap but TrueColor make an unshared one */
  status = XmuLookupStandardColormap(dpy, vi->screen, vi->visualid,
                vi->depth, XA_RGB_DEFAULT_MAP, False, True);
  if (status == 1) {
    status = XGetRGBColormaps(dpy, RootWindow(dpy, vi->screen),
                &standardCmaps, &numCmaps, XA_RGB_DEFAULT_MAP);

    if (status == 1)
      for (i = 0; i < numCmaps; i++)
        if (standardCmaps[i].visualid == vi->visualid) {
          cmap = standardCmaps[i].colormap;
          XFree(standardCmaps);
          return cmap;
        }
  }

  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen),
                vi->visual, AllocNone);
  return cmap;
}

/*******************************************************************************
 * phg_wsx_open_gl_display
 *
 * DESCR:	Open dipslay with OpenGL extension
 * RETURNS:	N/A
 */

Display* phg_wsx_open_gl_display(
   char *name,
   Pint *err_ind
   )
{
   Display *display;
   char *display_name;

   display_name = XDisplayName(name);
   display = XOpenDisplay(display_name);
   if (display != NULL) {
      if (!glXQueryExtension(display, NULL, NULL)) {
         XCloseDisplay(display);
         display = NULL;
         *err_ind = ERRN201;
      }
   }
   else {
      *err_ind = ERRN200;
   }

   return display;
}

/*******************************************************************************
 * phg_wsx_set_best_args
 *
 * DESCR:	Find best matching visual
 * RETURNS:	status as true or false
 *              Fills args array
 */

int phg_wsx_set_best_args(
   Ws*  ws,
   Wst* wst,
   int* args,
   int* rargc,
   int* err_ind
   )
{
   int argc = 0;
   Display *dpy = ws->display;
   int status = 0;
   /* Select workstation type */
   switch (wst->ws_type) {
      case PWST_HCOPY_TRUE_TGA:
      case PWST_HCOPY_TRUE_RGBA_PNG:
      case PWST_HCOPY_TRUE_RGB_PNG:
      case PWST_HCOPY_TRUE_EPS:
      case PWST_HCOPY_TRUE_PDF:
      case PWST_HCOPY_TRUE_SVG:
          args[argc++] = GLX_RENDER_TYPE;
          args[argc++] = GLX_RGBA_BIT;
          args[argc++] = GLX_DRAWABLE_TYPE;
          args[argc++] = GLX_WINDOW_BIT|GLX_PIXMAP_BIT;
          args[argc++] = GLX_RED_SIZE;
            args[argc++] = 8;
          args[argc++] = GLX_GREEN_SIZE;
            args[argc++] = 8;
          args[argc++] = GLX_BLUE_SIZE;
            args[argc++] = 8;
          args[argc++] = GLX_ALPHA_SIZE;
            args[argc++] = 8;
          args[argc++] = GLX_DEPTH_SIZE;
          args[argc++] = 24;
          args[argc] = None;
          status = 2;
          break;
      case PWST_OUTPUT_TRUE:
      case PWST_OUTIN_TRUE:
          args[argc++] = GLX_RGBA;
          args[argc++] = GLX_RED_SIZE;
             args[argc++] = 1;
          args[argc++] = GLX_GREEN_SIZE;
             args[argc++] = 1;
          args[argc++] = GLX_BLUE_SIZE;
             args[argc++] = 1;
          args[argc++] = GLX_DEPTH_SIZE;
             args[argc++] = 16;
          args[argc] = None;
          status = 1;
          break;
       case PWST_OUTPUT_TRUE_DB:
       case PWST_OUTIN_TRUE_DB:
          args[argc++] = GLX_DOUBLEBUFFER;
          args[argc++] = GLX_RGBA;
          args[argc++] = GLX_RED_SIZE;
             args[argc++] = 1;
          args[argc++] = GLX_GREEN_SIZE;
             args[argc++] = 1;
          args[argc++] = GLX_BLUE_SIZE;
             args[argc++] = 1;
          args[argc++] = GLX_DEPTH_SIZE;
             args[argc++] = 16;
          args[argc] = None;
          status = 1;
          break;

      default:
         *err_ind = ERR52;
         status = 0;
         break;
   }
   *rargc = argc;
   return(status);
}

/*******************************************************************************
 * phg_wsx_find_best_visual
 *
 * DESCR:	Find best matching visual
 * RETURNS:	N/A
 */

void phg_wsx_find_best_visual(
   Ws *ws,
   Wst *wst,
   XVisualInfo **visual_info,
   Colormap *cmap,
   Pint *err_ind
   )
{
   int args[20];
   int argc = 0;
   Display *dpy = ws->display;
   int nfb;
   int status;

   *err_ind = 0;
   status = phg_wsx_set_best_args(ws, wst, args, &argc, err_ind);
   switch (status)
     {
     case 1:
       *visual_info = glXChooseVisual(dpy, DefaultScreen(dpy), args);
       if (*visual_info == NULL) {
         *err_ind = ERRN205;
         printf("ERROR: Failed to get visual info\n");
       } else {
         /* NOTE: Only call this for true colour */
         *cmap = get_sharable_colormap(*visual_info, dpy);
         *err_ind = 0;
       }
       break;
     case 2:
       nfb = 0;
       ws->fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), args, &nfb);
       if (ws->fbc != NULL){
         *visual_info = glXGetVisualFromFBConfig(dpy, ws->fbc[0]);
         *err_ind = 0;
       } else {
         printf("ERROR: Failed to get visual info for frame buffer\n");
         *err_ind = ERRN205;
       }
       break;
     }
}

/*******************************************************************************
 * phg_wsx_create_context
 *
 * DESCR:	Create rendering context from visual info
 * RETURNS:	N/A
 */

GLXContext phg_wsx_create_context(
   Ws *ws,
   XVisualInfo *visual_info,
   Pint *err_ind
   )
{
   Display *dpy = ws->display;
   GLXContext context;

   context = glXCreateContext(dpy, visual_info, NULL, True);
   if (context == NULL) {
      *err_ind = ERRN206;
   }
   else {
      *err_ind = 0;
   }

   return context;
}

/*******************************************************************************
 * phg_wsx_pixel_colour
 *
 * DESCR:	Get colour from pixel value
 * RETURNS:	N/A
 */

void phg_wsx_pixel_colour(
   Ws *ws,
   Colormap cmap,
   unsigned long pixel,
   Pgcolr *gcolr
   )
{
   XColor color;
   Phg_ret ret;

   color.pixel = pixel;
   if (ws->display) {
     XQueryColor(ws->display, cmap, &color);
   } else {
     printf("ERROR: OpenPHIGS DISPLAY is not set");
   }
   gcolr->type = PMODEL_RGB;
   gcolr->val.general.x = (float) color.red / 65535.0;
   gcolr->val.general.y = (float) color.green / 65535.0;
   gcolr->val.general.z = (float) color.blue / 65535.0;
}

/*******************************************************************************
 * phg_wsx_update_ws_rect
 *
 * DESCR:	Update workstation rectangle
 * RETURNS:	N/A
 */

void phg_wsx_update_ws_rect(
   Ws *ws
   )
{
   XWindowAttributes wattr;
#ifdef DEBUGINP
  printf("DEBUG: Setting window attributes.\n");
#endif
  memset(&wattr, 0, sizeof(XWindowAttributes));
  XGetWindowAttributes(ws->display, ws->drawable_id, &wattr);
   WS_SET_WS_RECT(ws, &wattr)
}

/*******************************************************************************
 * phg_wsx_compute_ws_transform
 *
 * DESCR:       Compute workstation transform
 * RETURNS:     N/A
 */

void phg_wsx_compute_ws_transform(
   Plimit3 *ws_win,
   Plimit3 *ws_vp,
   Ws_xform *ws_xform
   )
{
   Pfloat sx, sy, sz, sxy;

   sx = (ws_vp->x_max - ws_vp->x_min) / (ws_win->x_max - ws_win->x_min);
   sy = (ws_vp->y_max - ws_vp->y_min) / (ws_win->y_max - ws_win->y_min);
   sz = (ws_vp->z_max - ws_vp->z_min) / (ws_win->z_max - ws_win->z_min);

   sxy = (sx < sy) ? sx : sy;

   ws_xform->scale.x = ws_xform->scale.y = sxy;
   ws_xform->scale.z = sz;

   ws_xform->offset.x = ws_vp->x_min - (ws_win->x_min * sxy);
   ws_xform->offset.y = ws_vp->y_min - (ws_win->y_min * sxy);
   ws_xform->offset.z = ws_vp->z_min - (ws_win->z_min * sz);
}
