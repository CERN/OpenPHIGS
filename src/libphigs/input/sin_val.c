/******************************************************************************

Copyright (c) 1989, 1990, 1991  X Consortium
Copyright (c) 2014 Surplus Users Ham Society
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

******************************************************************************/

/*
 * Default device specific routines for input.
 */

#include <stdio.h>
#include <stdlib.h>

#include "phg.h"
#include "ws_type.h"
#include "sin.h"
#include "private/sinP.h"

#ifndef GTK4_EXT
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#endif
#ifdef MOTIF
#include <Xm/Frame.h>
#include <Xm/Scale.h>
#include <Xm/RowColumn.h>
#include <Xm/PanedW.h>
#include <Xm/Label.h>
#else
#ifndef GTK4_EXT
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Box.h>
#endif
#endif
/*******************************************************************************
 * init_valuator
 *
 * DESCR:       init device
 * RETURNS:     N/A
 */
static int init_valuator(
		 Sin_input_device *device,
		 Sin_dev_init_data *new_data
){
#ifdef DEBUGINP
  printf("init_valuator called.\n");
#endif
  device->item_handle.valuator.pane = NULL;
  device->item_handle.valuator.box = NULL;
  device->item_handle.valuator.shell = NULL;
  device->item_handle.valuator.label = NULL;
  device->item_handle.valuator.value = NULL;
  device->item_handle.valuator.low = NULL;
  device->item_handle.valuator.scrollbar = NULL;
  device->item_handle.valuator.high = NULL;
  return(0);
}

/*******************************************************************************
 * set_float_arg
 *
 * DESCR:       Float argument helper function
 * RETURNS:     N/A
 */

static void set_float_arg(
    Arg	*args,
    Cardinal *num_args,
    String arg,
    float *val_p
    )
{
    if (sizeof(float) > sizeof(XtArgVal)) {
	XtSetArg(args[*num_args], arg, (XtArgVal)val_p); (*num_args)++;
    } else {
	XtArgVal	*arg_val = (XtArgVal *)val_p;
	XtSetArg(args[*num_args], arg, *arg_val); (*num_args)++;
    }
}

/*******************************************************************************
 * valuator_jump
 *
 * DESCR:       Jump argument helper function
 * RETURNS:     N/A
 */

static void valuator_jump(
    Widget w,
    XtPointer client_data,
    XtPointer call_data
    )
{
    Arg	args[2];
    Cardinal num_args = 0;
    char buf[256];
    float percent;
    float delta, prec;
    Sin_input_device *device = (Sin_input_device *)client_data;
    Sin_valuator_device_data *data = &device->data.valuator;
#ifdef GTK4_EXT
    data->value = gtk_range_get_value(GTK_RANGE(device->item_handle.valuator.scrollbar));
#elif defined(MOTIF)
    XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *) call_data;
    delta = data->high - data->low;
    if (delta<=1.0) prec = 100.0;
    if (delta>1.0 && delta<=10.0) prec=10.;
    if (delta>10.) prec=1.;

    percent = cbs->value/prec;
    data->value = percent;
#else
    percent = *((float *) call_data);
    data->value = data->low + percent * (data->high - data->low);
    if ( device->item_handle.valuator.value ) {
	sprintf( buf, data->format, data->value );
	XtSetArg(args[num_args], XtNlabel, (XtArgVal)buf); num_args++;
	XtSetValues( device->item_handle.valuator.value, (ArgList)args,
	    num_args );
    }
#endif
    if ( device->mode == SIN_EVENT ) {
	unsigned	status;
	status = phg_sin_ws_enque_events( 1, &device );
	if ( SIN_EVENT_NOT_ENQUED(status) )
	    XBell( XtDisplay(w), 0 );
    }
    /* The action proc takes care of REQUEST.  No additional action needed
     * for SAMPLE.
     */
}

/*******************************************************************************
 * enable_valuator
 *
 * DESCR:       Enable valuator callback
 * RETURNS:     N/A
 */

static void enable_valuator(
    Sin_input_device *device
    )
{
    Arg	args[20];
    Cardinal num_args = 0;
    char buf[256];
    float init_value, fac, delta;
    int prec;
    int imin, imax, iinit;
    Widget w;
    Ws *ws;
    Sin_valuator_device_data *data = &device->data.valuator;
    // this shell seems to be in common for all devices
    Widget parent = device->ws->shell;
    // we also have device->ws->wsh which is the work station handle. There we may be able to store some global stuff
    // wsh is a _Ws*, and _Ws is defined in ws.h
    /*
    if (device->item_handle.valuator.shell != NULL){
      printf("Valuator shell for device %d already defined.Ignoring call.\n", device->num);
      return;
    }
    */
    ws = device->ws->wsh;
#ifdef DEBUGINP
    printf("\n#### enable valuator device at %lx####\n", (unsigned long)device);
    printf("Device number: %d\n", device->num);
    printf("Valuator type %d\n", device->pe_type);
    printf("Area ll(%d %d) up(%d %d)\n",
	   device->echo_area.ll.x,
	   device->echo_area.ll.y,
	   device->echo_area.ur.x,
	   device->echo_area.ur.y);
    printf("data is at 0x%lx\n", (unsigned long)data);
    printf("Parent:    0x%lx\n", (unsigned long)parent);
    printf("Top Level: 0x%lx\n", (unsigned long)ws->top_level);
    printf("Shell:     0x%lx\n", (unsigned long)ws->shell);
    printf("ValBox:    0x%lx\n", (unsigned long)ws->valuator_box);
    printf("Number of boxed valuators so far is %d\n", ws->num_boxed_valuators);
    printf("Number of already configured is %d\n", data->num_boxed);
#endif
    if (device->item_handle.valuator.pane != NULL){
      return;
    }
#ifdef GTK4_EXT
    if (device->item_handle.valuator.shell == NULL) {
      ws->valuator_shell = (Widget)gtk_window_new();
      gtk_window_set_title(GTK_WINDOW(ws->valuator_shell), "Valuator");
      
      GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
      gtk_window_set_child(GTK_WINDOW(ws->valuator_shell), box);
      ws->valuator_box = (Widget)box;

      if (data->label) {
          GtkWidget *lbl = gtk_label_new(data->label);
          gtk_box_append(GTK_BOX(box), lbl);
      }

      device->item_handle.valuator.scrollbar = (Widget)gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, data->low, data->high, (data->high - data->low)/100.0);
      gtk_range_set_value(GTK_RANGE(device->item_handle.valuator.scrollbar), data->init_value);
      g_signal_connect(device->item_handle.valuator.scrollbar, "value-changed", G_CALLBACK(valuator_jump), device);
      gtk_box_append(GTK_BOX(box), (GtkWidget*)device->item_handle.valuator.scrollbar);
      
      gtk_window_present(GTK_WINDOW(ws->valuator_shell));
    }
    device->item_handle.valuator.shell = ws->valuator_shell;
    device->item_handle.valuator.pane = ws->valuator_box;
#elif defined(MOTIF)
    XmString label;
    /* Create the containing shell. */
    sprintf( buf, "valuator%d", device->num );
    if ( ws->valuator_shell == NULL){
      /*
	Note: we can use overrideShellWidgetClass to avoid this to be managed by the WM
	While the test works with this, in grope valutors don't work so we don't for now
      */
      ws->valuator_shell =
	XtVaAppCreateShell( buf, NULL, applicationShellWidgetClass,
			    ws->display,
			    XtNx, (XtArgVal)SIN_EA_X(&device->echo_area),
			    XtNy, (XtArgVal)SIN_EA_Y(&device->echo_area),
			    NULL );
      ws->valuator_frame =
	XtVaCreateManagedWidget( "vfame", xmFrameWidgetClass,
				 ws->valuator_shell,
				 NULL );
      ws->valuator_box =
	XtVaCreateManagedWidget( "box", xmRowColumnWidgetClass,
				 ws->valuator_frame,
				 NULL );
    }
    if (device->item_handle.valuator.pane != NULL){
      XtDestroyWidget(device->item_handle.valuator.pane);
    }
    sprintf( buf, "pane%d", device->num );
    device->item_handle.valuator.pane =
      XtVaCreateManagedWidget( buf, xmPanedWindowWidgetClass, ws->valuator_box,
			       XmNorientation, XmHORIZONTAL,
			       XtNwidth, (XtArgVal)abs(SIN_EA_WIDTH(&device->echo_area)),
			       XtNheight, (XtArgVal)abs(SIN_EA_HEIGHT(&device->echo_area)),
			       XtNx, (XtArgVal)SIN_EA_X(&device->echo_area),
			       XtNy, (XtArgVal)SIN_EA_Y(&device->echo_area),
			       NULL );
    device->item_handle.valuator.box = ws->valuator_box;
    device->item_handle.valuator.shell = ws->valuator_shell;

    /* Create the label. */
    if ( data->label && data->label[0] ) {
      if (device->item_handle.valuator.label == NULL){
	label = XmStringCreateLocalized (data->label);
	device->item_handle.valuator.label =
	  XtVaCreateManagedWidget( "label", xmLabelWidgetClass,
				   device->item_handle.valuator.pane,
				   XmNlabelString, label,
				   NULL );
	XmStringFree(label);
      }
    }

    /* Create the value readout. */
    /*
    if ( device->echo_sw && data->format && data->format[0] ) {
      sprintf( buf, data->format, data->value );
      if (device->item_handle.valuator.value == NULL)
	device->item_handle.valuator.value =
	  XtVaCreateManagedWidget( "readout", labelWidgetClass,
				   device->item_handle.valuator.pane,
				   XtNlabel, (XtArgVal)buf,
				   NULL );
    }
    */
    /* Create the low range limit label. */
    if ( data->low_label && data->low_label[0] ) {
      sprintf( buf, data->low_label, data->low );
      if (device->item_handle.valuator.low == NULL){
	label = XmStringCreateLocalized (buf);
	device->item_handle.valuator.low =
	  XtVaCreateManagedWidget( "low_label", xmLabelWidgetClass,
				   device->item_handle.valuator.pane,
				   XmNlabelString, label,
				   NULL );
	XmStringFree(label);
      }
    }

    /* Create the valuator */
    //num_args = 0;
    delta = data->high - data->low;
    fac = 100.0;
    if (delta<=1){
      prec = 2;
    }
    if (delta>1 && delta<=10){
      prec=1;
    }
    if (delta>10){
      prec=0;
    }
    if (delta > 10){
      iinit = (int)data->init_value;
      imin = (int)(data->low);
      imax = (int)(data->high);
    } else {
      iinit = (int)(fac*(data->init_value - data->low) / delta);
      imin = (int)(fac*data->low / delta);
      imax = (int)(fac*data->high / delta);
    }
    //set_float_arg( args, &num_args, XtNtopOfThumb, &init_value );
    if (device->item_handle.valuator.scrollbar == NULL){
      device->item_handle.valuator.scrollbar =
	XtVaCreateManagedWidget( "scale", xmScaleWidgetClass, device->item_handle.valuator.pane,
				 XmNorientation,XmHORIZONTAL,
				 XmNminimum, imin,
				 XmNmaximum, imax,
				 XmNvalue, iinit,
				 XmNshowValue, True,
				 XmNdecimalPoints, prec,
				 NULL);
      XtAddCallback( device->item_handle.valuator.scrollbar,
		     XmNdragCallback, valuator_jump, (XtPointer)device);
    }
    /* Create the high range limit label. */
    if ( data->high_label && data->high_label[0] ) {
      sprintf( buf, data->high_label, data->high );
      if (device->item_handle.valuator.high == NULL){
	label = XmStringCreateLocalized (buf);
	device->item_handle.valuator.high =
	  XtVaCreateManagedWidget( "high_label", xmLabelWidgetClass,
				   device->item_handle.valuator.pane,
				   XmNlabelString, label,
				   NULL );
	XmStringFree(label);
      }
    }
#else
    /* Create the containing shell. */
    sprintf( buf, "valuator%d", device->num );
    if (abs(device->pe_type)==3) {
      if ( ws->valuator_shell == NULL){
	/*
	   Note: we can use overrideShellWidgetClass to avoid this to be managed by the WM
	   While the test works with this, in grope valutors don't work so we don't for now
	 */
	ws->valuator_shell =
	  XtVaAppCreateShell( NULL, NULL, applicationShellWidgetClass,
			      ws->display,
			      XtNx, (XtArgVal)SIN_EA_X(&device->echo_area),
			      XtNy, (XtArgVal)SIN_EA_Y(&device->echo_area),
			      NULL );
	ws->valuator_box =
	  XtVaCreateManagedWidget( "box", boxWidgetClass,
				   ws->valuator_shell,
				   NULL );
      }
      sprintf( buf, "pane%d", device->num );
      if (device->item_handle.valuator.pane != NULL){
	XtDestroyWidget(device->item_handle.valuator.pane);
      }
      device->item_handle.valuator.pane =
	XtVaCreateManagedWidget( buf, panedWidgetClass,
				 ws->valuator_box,
				 XtNwidth, (XtArgVal)abs(SIN_EA_WIDTH(&device->echo_area)),
				 XtNheight, (XtArgVal)abs(SIN_EA_HEIGHT(&device->echo_area)),
				 XtNx, (XtArgVal)SIN_EA_X(&device->echo_area),
				 XtNy, (XtArgVal)SIN_EA_Y(&device->echo_area),
				 NULL );
      device->item_handle.valuator.box = ws->valuator_box;
      device->item_handle.valuator.shell = ws->valuator_shell;

    } else {
      if (device->item_handle.valuator.shell == NULL){
	device->item_handle.valuator.shell =
	  XtVaCreatePopupShell( buf, applicationShellWidgetClass,
				parent,
				XtNoverrideRedirect, False,
				XtNwidth, (XtArgVal)abs(SIN_EA_WIDTH(&device->echo_area)),
				XtNheight, (XtArgVal)abs(SIN_EA_HEIGHT(&device->echo_area)),
				XtNx, (XtArgVal)SIN_EA_X(&device->echo_area),
				XtNy, (XtArgVal)SIN_EA_Y(&device->echo_area),
				NULL );
	// Create the containing pane.
	device->item_handle.valuator.pane =
	  XtVaCreateManagedWidget( "pane", panedWidgetClass,
				   device->item_handle.valuator.shell,
				   NULL );
      }
    }

    /* Create the label. */
    if ( data->label && data->label[0] ) {
#ifdef DEBUGINP
      printf("VALUATOR: Label is \"%s\" at 0x%lx\n",data->label, (unsigned long)data->label);
#endif
      if (device->item_handle.valuator.label == NULL)
	device->item_handle.valuator.label =
	  XtVaCreateManagedWidget( "label", labelWidgetClass,
				   device->item_handle.valuator.pane,
				   XtNlabel, (XtArgVal)data->label,
				   NULL );
#ifdef DEBUGINP
    } else {
      printf("VALUATOR: NO label defined!\n");
#endif
    }

    /* Create the value readout. */
    if ( device->echo_sw && data->format && data->format[0] ) {
      sprintf( buf, data->format, data->value );
      if (device->item_handle.valuator.value == NULL)
	device->item_handle.valuator.value =
	  XtVaCreateManagedWidget( "readout", labelWidgetClass,
				   device->item_handle.valuator.pane,
				   XtNlabel, (XtArgVal)buf,
				   NULL );
    }

    /* Create the low range limit label. */
    if ( data->low_label && data->low_label[0] ) {
      sprintf( buf, data->low_label, data->low );
      if (device->item_handle.valuator.low == NULL)
	device->item_handle.valuator.low =
	  XtVaCreateManagedWidget( "low_label", labelWidgetClass,
				   device->item_handle.valuator.pane,
				   XtNlabel, (XtArgVal)buf,
				   NULL );
    }

    /* Create the scroll bar. */
    num_args = 0;
    init_value = (data->init_value - data->low) / (data->high - data->low);
    set_float_arg( args, &num_args, XtNtopOfThumb, &init_value );
    if (device->item_handle.valuator.scrollbar == NULL){
      w = device->item_handle.valuator.scrollbar =
	XtCreateManagedWidget( "scrollbar", scrollbarWidgetClass,
			       device->item_handle.valuator.pane, args, num_args );
      XtAddCallback( device->item_handle.valuator.scrollbar,
		     XtNjumpProc, valuator_jump, (XtPointer)device);
    }
    /* Create the high range limit label. */
    if ( data->high_label && data->high_label[0] ) {
      sprintf( buf, data->high_label, data->high );
      if (device->item_handle.valuator.high == NULL)
	device->item_handle.valuator.high =
	  XtVaCreateManagedWidget( "high_label", labelWidgetClass,
				   device->item_handle.valuator.pane,
				   XtNlabel, (XtArgVal)buf,
				   NULL );
    }
#endif
    /* done configuring the device so lets bump the number of boxed valuators */
    ws->num_boxed_valuators += 1;
    if (abs(device->pe_type)!=3 || ws->num_boxed_valuators == data->num_boxed) {
#ifdef DEBUGINP
      printf("Popping up %d shell at 0x%lx\n", device->num, (unsigned long)device->item_handle.valuator.shell);
#endif
      XtManageChild(ws->valuator_box);
      XtPopup( device->item_handle.valuator.shell, XtGrabNone );
      if ( device->mode == SIN_REQUEST_PENDING )
	XSaveContext( XtDisplay(w), XtWindow(w), phg_sin_device_context_id,
		      (caddr_t)device );
    }
#ifdef DEBUGINP
      printf("Done with enable valuator\n\n");
#endif

}

/*******************************************************************************
 * disable_valuator
 *
 * DESCR:       Disable valuator callback
 * RETURNS:     N/A
 */

static void disable_valuator(
    Sin_input_device *device
    )
{
#ifdef DEBUGINP
      printf("Disable valuator called\n\n");
#endif
  Widget w = device->item_handle.valuator.scrollbar;
  Ws *ws;
  ws = device->ws->wsh;
  if (abs(device->pe_type)==3) {
    ws->num_boxed_valuators -= 1;
    if (ws->num_boxed_valuators > 0){
#ifdef DEBUGINP
      printf("Will not destroy the widget yet\n\n");
#endif
      //XtDestroyWidget( device->item_handle.valuator.pane );
      //device->item_handle.valuator.pane = NULL;
      //XtManageChild(ws->valuator_box);
    } else {
      XDeleteContext( XtDisplay(w), XtWindow(w), phg_sin_device_context_id );
#ifdef DEBUGINP
      printf("Destroy the whole valuator shell\n\n");
#endif
      ws->num_boxed_valuators = 0;
      XtPopdown(ws->valuator_shell);
      XtDestroyWidget( ws->valuator_shell);
      ws->valuator_shell = NULL;
      ws->valuator_frame = NULL;
      ws->valuator_box = NULL;
    }
  } else {
    XDeleteContext( XtDisplay(w), XtWindow(w), phg_sin_device_context_id );
    XtDestroyWidget( device->item_handle.valuator.shell );
    device->item_handle.valuator.shell = NULL;
  }
  device->item_handle.valuator.label = NULL;
  device->item_handle.valuator.value = NULL;
  device->item_handle.valuator.low = NULL;
  device->item_handle.valuator.scrollbar = NULL;
  device->item_handle.valuator.high = NULL;
}

/*******************************************************************************
 * phg_sin_dev_boot_valuator
 *
 * DESCR:       Boot valuator device
 * RETURNS:     N/A
 */

void phg_sin_dev_boot_valuator(
			       Sin_input_device *dev
			       )
{
  switch ( dev->data.valuator.type ) {
  case WST_VAL_TYPE_SLIDER:
    dev->dev_ops.reset = NULL;
    dev->dev_ops.sample = NULL;
    dev->dev_ops.resize = NULL;
    dev->dev_ops.repaint = NULL;
    dev->dev_ops.destroy = NULL;
    dev->dev_ops.create = NULL;
    dev->dev_ops.init = init_valuator;
    dev->dev_ops.enable = enable_valuator;
    dev->dev_ops.disable = disable_valuator;
    break;
  default:
    printf("sin_val.c: Attempt to boot unknown valuator type %d\n", dev->data.valuator.type);
    break;
  }
}
