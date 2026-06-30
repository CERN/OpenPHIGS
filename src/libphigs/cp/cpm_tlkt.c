/* $XConsortium: cpm_tlkt.c,v 5.4 94/04/17 20:41:21 hersh Exp $ */

/***********************************************************

Copyright (c) 1989, 1990, 1991  X Consortium
Copyright (c) 2022-2023 CERN

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

Copyright 1989, 1990, 1991 by Sun Microsystems, Inc.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Sun Microsystems,
not be used in advertising or publicity pertaining to distribution of
the software without specific, written prior permission.

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
#include <stdio.h>
#ifndef GTK4_EXT
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#endif
#include "phg.h"
#include "ws.h"
#include "sin.h"

#ifndef GTK4_EXT
/* TODO: Fill in the real options. */
static XrmOptionDescRec options[] = {
{"-horiz",	"scrollbar.orientation", XrmoptionNoArg,  "horizontal"}
};

static String fallbacks[] = {
    "*box.pane.orientation:  horizontal",
    "*box.pane1.orientation:  horizontal",
    "*box.pane2.orientation:  horizontal",
    "*box.pane3.orientation:  horizontal",
    "*box.pane4.orientation:  horizontal",
    "*box.pane5.orientation:  horizontal",
    "*box.pane6.orientation:  horizontal",
    "*box.pane7.orientation:  horizontal",
    "*box.pane8.orientation:  horizontal",
    "*box.pane9.orientation:  horizontal",
    "*box.pane10.orientation:  horizontal",
    "*box.pane11.orientation:  horizontal",
    "*box.pane12.orientation:  horizontal",
    "*box*scrollbar.orientation:  horizontal",
    "*box*scrollbar.length:  50",
    "*box*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*box*scrollbar.skipAdjust:  FALSE",
    "*box*skipAdjust:  TRUE",
    "*box*showGrip:  FALSE",

    "*valuator1.pane.orientation:  horizontal",
    "*valuator1*scrollbar.orientation:  horizontal",
    "*valuator1*scrollbar.length:  50",
    "*valuator1*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator1*scrollbar.skipAdjust:  FALSE",
    "*valuator1*skipAdjust:  TRUE",
    "*valuator1*showGrip:  FALSE",

    "*valuator2.pane.orientation:  horizontal",
    "*valuator2*scrollbar.orientation:  horizontal",
    "*valuator2*scrollbar.length:  50",
    "*valuator2*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator2*scrollbar.skipAdjust:  FALSE",
    "*valuator2*skipAdjust:  TRUE",
    "*valuator2*showGrip:  FALSE",

    "*valuator3.pane.orientation:  horizontal",
    "*valuator3*scrollbar.orientation:  horizontal",
    "*valuator3*scrollbar.length:  50",
    "*valuator3*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator3*scrollbar.skipAdjust:  FALSE",
    "*valuator3*skipAdjust:  TRUE",
    "*valuator3*showGrip:  FALSE",

    "*valuator4.pane.orientation:  horizontal",
    "*valuator4*scrollbar.orientation:  horizontal",
    "*valuator4*scrollbar.length:  50",
    "*valuator4*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator4*scrollbar.skipAdjust:  FALSE",
    "*valuator4*skipAdjust:  TRUE",
    "*valuator4*showGrip:  FALSE",

    "*valuator5.pane.orientation:  horizontal",
    "*valuator5*scrollbar.orientation:  horizontal",
    "*valuator5*scrollbar.length:  50",
    "*valuator5*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator5*scrollbar.skipAdjust:  FALSE",
    "*valuator5*skipAdjust:  TRUE",
    "*valuator5*showGrip:  FALSE",

    "*valuator6.pane.orientation:  horizontal",
    "*valuator6*scrollbar.orientation:  horizontal",
    "*valuator6*scrollbar.length:  50",
    "*valuator6*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator6*scrollbar.skipAdjust:  FALSE",
    "*valuator6*skipAdjust:  TRUE",
    "*valuator6*showGrip:  FALSE",

    "*valuator7.pane.orientation:  horizontal",
    "*valuator7*scrollbar.orientation:  horizontal",
    "*valuator7*scrollbar.length:  50",
    "*valuator7*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator7*scrollbar.skipAdjust:  FALSE",
    "*valuator7*skipAdjust:  TRUE",
    "*valuator7*showGrip:  FALSE",

    "*valuator8.pane.orientation:  horizontal",
    "*valuator8*scrollbar.orientation:  horizontal",
    "*valuator8*scrollbar.length:  50",
    "*valuator8*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator8*scrollbar.skipAdjust:  FALSE",
    "*valuator8*skipAdjust:  TRUE",
    "*valuator8*showGrip:  FALSE",

    "*valuator9.pane.orientation:  horizontal",
    "*valuator9*scrollbar.orientation:  horizontal",
    "*valuator9*scrollbar.length:  50",
    "*valuator9*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator9*scrollbar.skipAdjust:  FALSE",
    "*valuator9*skipAdjust:  TRUE",
    "*valuator9*showGrip:  FALSE",

    "*valuator10.pane.orientation:  horizontal",
    "*valuator10*scrollbar.orientation:  horizontal",
    "*valuator10*scrollbar.length:  50",
    "*valuator10*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator10*scrollbar.skipAdjust:  FALSE",
    "*valuator10*skipAdjust:  TRUE",
    "*valuator10*showGrip:  FALSE",

    "*valuator11.pane.orientation:  horizontal",
    "*valuator11*scrollbar.orientation:  horizontal",
    "*valuator11*scrollbar.length:  50",
    "*valuator11*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator11*scrollbar.skipAdjust:  FALSE",
    "*valuator11*skipAdjust:  TRUE",
    "*valuator11*showGrip:  FALSE",

    "*valuator12.pane.orientation:  horizontal",
    "*valuator12*scrollbar.orientation:  horizontal",
    "*valuator12*scrollbar.length:  50",
    "*valuator12*scrollbar.translations: \
          <Btn1Down>:	StartScroll(Continuous) MoveThumb() NotifyThumb() \\n\
	  <Btn1Motion>:	MoveThumb() NotifyThumb() \\n\
	  <Btn1Up>:	NotifyScroll(Proportional) RequestSatisfied() \
			EndScroll() \
	  ",
    "*valuator12*scrollbar.skipAdjust:  FALSE",
    "*valuator12*skipAdjust:  TRUE",
    "*valuator12*showGrip:  FALSE",

    "*choice1.viewport.height: 50",
    "*choice1.viewport.width:  300",
    "*choice1*allowHoriz:  TRUE",
    "*choice1*allowVert:  TRUE",
    "*choice1*list.translations: \
          <Btn1Down>,<Btn1Up>:	Set() Notify() RequestSatisfied() \
	  ",

    "*choice2.viewport.height: 50",
    "*choice2.viewport.width:  300",
    "*choice2*allowHoriz:  TRUE",
    "*choice2*allowVert:  TRUE",
    "*choice2*list.translations: \
          <Btn1Down>,<Btn1Up>:	Set() Notify() RequestSatisfied() \
	  ",

    "*choice3.viewport.height: 50",
    "*choice3.viewport.width:  300",
    "*choice3*allowHoriz:  TRUE",
    "*choice3*allowVert:  TRUE",
    "*choice3*list.translations: \
          <Btn1Down>,<Btn1Up>:	Set() Notify() RequestSatisfied() \
	  ",

    "*string1*text.width: 400",
    "*string1*text.height: 50",
    "*string1*text.editType:          edit",
    "*string1*text.scrollVertical:    never",
    "*string1*text.scrollHorizontal:  never",
    "*string1*text.autoFill:          on",

    "*string2*text.width: 		400",
    "*string2*text.height: 		50",
    "*string2*text.editType:          edit",
    "*string2*text.scrollVertical:    never",
    "*string2*text.scrollHorizontal:  never",
    "*string2*text.autoFill:          on",

    "*string3*text.width: 		400",
    "*string3*text.height: 		50",
    "*string3*text.editType:          edit",
    "*string3*text.scrollVertical:    never",
    "*string3*text.scrollHorizontal:  never",
    "*string3*text.autoFill:          on",

    "*message.box.orientation:		vertical",
    "*message.box.allowResize:		True",
    "*message*label.resize:		True",
    "*message*button.label:		Done",

    NULL,
};

static XtActionsRec	actions[] = {
    {"RequestSatisfied",	(XtActionProc)phg_sin_xt_request_satisfied},
    {"StringEvent",		(XtActionProc)phg_sin_xt_string_event},
};

XContext        phg_sin_device_context_id;
#endif

/* No need for a "destroy_toolkit function since all the resources are
 * destroyed when the PM terminates.
 */

XtAppContext phg_cpm_init_toolkit( int argc, char **argv )
{
  XtAppContext app_con = NULL;

#ifdef GTK4_EXT
  gtk_init();
#else
  XtToolkitInitialize();
#ifdef DEBUGINP
  printf("Initialising Xt Toolkit\n");
#endif
  if ( (app_con = XtCreateApplicationContext()) ) {
#ifdef DEBUGINP
    printf("Configuring fallbacks\n");
#endif
    XtAppSetFallbackResources( app_con, fallbacks );
    XtAppAddActions( app_con, actions, XtNumber(actions) );
    phg_sin_device_context_id = XUniqueContext();
  }
#endif
  return app_con;
}

Widget phg_cpm_toolkit_add_connection( XtAppContext context, Display* display, Pint* err )
{
  int    argc = 0, status = 0;
  char   **argv = (char **)NULL;
  Widget top_level = NULL;

  *err = 0;
#ifdef GTK4_EXT
  /* In GTK4, Display connection is handled by gtk_init() or implicitly. */
  /* We can create a dummy or invisible top-level window if needed, but 
   * OpenPHIGS handles GLX on X11 windows directly. We just need to return 
   * a valid pointer to indicate success if the caller checks it. */
  top_level = (Widget)gtk_window_new();
#else
  /* TODO: XtDisplayInitialize destroys the arg vector, but we need to
   * keep it for the next display initialized.  So copy it to scratch
   * space then pass the copy.
   */
  XtDisplayInitialize( context, display,
        "Workstation", NULL, NULL, 0,
        &argc, argv );
  top_level = XtVaAppCreateShell( NULL, NULL, applicationShellWidgetClass,
				  display, NULL );
  if (top_level == NULL)
    *err = ERRN204;
#endif
  return top_level;
}

int phg_cpm_toolkit_open_ws( Ws *ws ) {
    char	buf[100];
    int		status = 0;

#ifdef DEBUGINP
    printf("cpm tookit open WS\n");
#endif
    status = 0;
    sprintf( buf, "workstation%d", ws->id );
#ifdef GTK4_EXT
    ws->shell = (Widget)gtk_window_new();
    if (ws->shell == NULL){
      status = 1;
    }
#else
    ws->shell = XtVaCreatePopupShell( buf, applicationShellWidgetClass,
				      ws->top_level, NULL);
    if (ws->shell == NULL){
      printf("Failed to create shell for %s! Top_level was %ld\n", buf, (long)ws->top_level);
      status = 1;
    }
#endif

    return status;
}

void phg_cpm_toolkit_close_ws( Ws *ws ) {
#ifdef GTK4_EXT
    if (ws->shell) {
      gtk_window_destroy(GTK_WINDOW(ws->shell));
      ws->shell = NULL;
    }
#else
    XtDestroyWidget( ws->shell );
#endif
}
