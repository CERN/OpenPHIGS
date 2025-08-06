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

#ifdef GLEW
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif

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
  char buffer[1024];
  XGetErrorText(display, error->error_code, buffer, 1024);
  printf("An X11 error was detected: code: %d: %s\n", error->error_code, buffer);
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
 * phg_wsx_setup_tool_nodisp
 *
 * DESCR:       Create invisible window
 * RETURNS:     TRUE or FALSE
 */

int phg_wsx_setup_tool_nodisp(
                              Ws *ws,
                              Phg_args_conn_info *conn_info,
                              Phg_args_open_ws *args
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
  Wst *wst = args->type;
  Wst_xwin_dt *xdt = &wst->desc_tbl.xwin_dt;
  int         argc = 0;
  char        **argv = (char **)NULL;
  int screen;

  XSetErrorHandler(X11ErrorHandler);

  screen = DefaultScreen(ws->display);
  Display *display = ws->display;

  /* Find matching visual */
  status = TRUE;

  /* save the current context */
  glGetIntegerv(GL_VIEWPORT, ws->old_viewport);
#ifdef DEBUG
  printf("Saved view port %d %d %d %d\n", ws->old_viewport[0], ws->old_viewport[1], ws->old_viewport[2], ws->old_viewport[3]);
#endif
  phg_wsx_find_best_visual(ws, wst, &best_info, &cmap, &err_ind);
  if (err_ind != 0) {
    ERR_BUF(ws->erh, err_ind);
    status = FALSE;
  }
  else {
    attrs.colormap = cmap;
    attrs.border_pixel = WhitePixel(display, screen);
    attrs.background_pixel = BlackPixel(display, screen);
    ws->glx_context = 0;
    drawable_id = 0;
    glGenFramebuffers(1, &(ws->fbuf));
    glBindFramebuffer(GL_FRAMEBUFFER, ws->fbuf);

    glGenTextures(1, &(ws->colorbuf));
    glBindTexture(GL_TEXTURE_2D, ws->colorbuf);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 args->width,
                 args->height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ws->colorbuf, 0);

    glGenRenderbuffers(1, &(ws->depthbuf));
    glBindRenderbuffer(GL_RENDERBUFFER, ws->depthbuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, args->width, args->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ws->depthbuf);
    //    glViewport(0, 0, args->width, args->height);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    /* check the status */
    GLenum fbstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (fbstatus) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      printf("Incomplete attachment\n");
      status = FALSE;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      printf("Missing attachment\n");
      status = FALSE;
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      printf("Unsupported framebuffer config\n");
      status = FALSE;
      break;
#ifdef DEBUG
    default:
      printf("FBO status: 0x%X\n", status);
#endif
    }
    /* clear the new buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Initialize rendering context */
    size_hints.flags = USPosition | USSize;
    size_hints.x = xdt->tool.x;
    size_hints.y = xdt->tool.y;
    size_hints.width = xdt->tool.width;
    size_hints.height = xdt->tool.height;
    ws->drawable_id = drawable_id;

    /* set background color defaut */
    uint8_t red   = (attrs.background_pixel >> 16) & 0xFF;
    uint8_t green = (attrs.background_pixel >> 8) & 0xFF;
    uint8_t blue  = attrs.background_pixel & 0xFF;
    background.type = PMODEL_RGB;
    background.val.general.x = (float) red / 65535.0;
    background.val.general.y = (float) green / 65535.0;
    background.val.general.z = (float) blue / 65535.0;

    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &(ws->fbuf));

    if (!wsgl_init(ws, &background, NUM_SELECTABLE_STRUCTS)) {
      ERR_BUF(ws->erh, ERR900);
      free(ws);
      status = FALSE;
    }
  }
  return status;
}

/*******************************************************************************
 * phg_wsx_cleanup_fb
 *
 * DESCR:       Release window
 * RETURNS:     N/A
 */
void phg_wsx_cleanup_fb(
                        Ws *ws
                        )
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteFramebuffers(1, &(ws->fbuf));
  glDeleteTextures(1, &(ws->colorbuf));
  glDeleteRenderbuffers(1, &(ws->depthbuf));
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
   if (ws->drawable_id != 0){
     XDestroyWindow(ws->display, ws->drawable_id);
   }
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
