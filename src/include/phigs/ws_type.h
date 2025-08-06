/***********************************************************

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
******************************************************************/

#ifndef _ws_type_h
#define _ws_type_h

#include "phigs.h"
#include "phgretdata.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Workstation types */
#define PWST_OUTPUT_TRUE                 0
#define PWST_OUTIN_TRUE                  1
#define PWST_OUTPUT_TRUE_DB              2
#define PWST_OUTIN_TRUE_DB               3
#define PWST_HCOPY_TRUE_TGA              4
#define PWST_HCOPY_TRUE_RGB_PNG          5
#define PWST_HCOPY_TRUE_RGBA_PNG         6
#define PWST_HCOPY_TRUE_EPS              7
#define PWST_HCOPY_TRUE_PDF              8
#define PWST_HCOPY_TRUE_SVG              9

/* Default tables */
#define WST_MIN_PREDEF_LINE_REPS         1
#define WST_MIN_PREDEF_MARKER_REPS       1
#define WST_MIN_PREDEF_TEXT_REPS         1
#define WST_MIN_PREDEF_INTERIOR_REPS     1
#define WST_MIN_PREDEF_EDGE_REPS         1
#define WST_MIN_PREDEF_VIEW_REPS         6

#define WST_MAX_NUM_PETS                 10
#define WST_MAX_NUM_LOCATOR_DEVS         3
#define WST_MAX_NUM_STROKE_DEVS          3
#define WST_MAX_NUM_PICK_DEVS            3
#define WST_MAX_NUM_VALUATOR_DEVS        12
#define WST_MAX_NUM_CHOICE_DEVS          3
#define WST_MAX_NUM_STRING_DEVS          3

/* Default settings */
#define WST_DEFAULT_VALUATOR_LABEL       "value:"
#define WST_DEFAULT_VALUATOR_FORMAT      "%8.3g"
#define WST_DEFAULT_VALUATOR_LOW_LABEL   "[%8.3g]"
#define WST_DEFAULT_VALUATOR_HIGH_LABEL  "[%8.3g]"

typedef enum {
   WST_LOC_TYPE_POINTER_BUTTON_1,
   WST_LOC_TYPE_POINTER_BUTTON_2,
   WST_LOC_TYPE_POINTER_BUTTON_3
} Wst_loc_type;

typedef enum {
   WST_PICK_TYPE_POINTER_BUTTON_1,
   WST_PICK_TYPE_POINTER_BUTTON_2,
   WST_PICK_TYPE_POINTER_BUTTON_3
} Wst_pick_type;

typedef enum {
   WST_STROKE_TYPE_POINTER_BUTTON_1,
   WST_STROKE_TYPE_POINTER_BUTTON_2,
   WST_STROKE_TYPE_POINTER_BUTTON_3
} Wst_stroke_type;

typedef enum {
   WST_VAL_TYPE_SLIDER
} Wst_val_type;

typedef enum {
   WST_CHOICE_TYPE_LIST
} Wst_choice_type;

typedef enum {
   WST_STRING_TYPE_WINDOW
} Wst_string_type;

typedef enum {
   WST_BOUND,
   WST_UNBOUND,
   WST_GLOBAL
} Wst_bound_status;

typedef struct {
   Ppoint3      position;
   Pint         num_pets;
   Pint         pets[WST_MAX_NUM_PETS];
   Plimit3      e_volume;
   Ploc_data3   record;
   Wst_loc_type type;
} Wst_defloc;

typedef struct {
   Ppath_order  order;
   Pint         num_pets;
   Pint         pets[WST_MAX_NUM_PETS];
   Plimit3      e_volume;
   Ppick_data3  record;
   Wst_pick_type type;
} Wst_defpick;

typedef struct {
   Pint            max_bufsize;
   Pint            num_pets;
   Pint            pets[WST_MAX_NUM_PETS];
   Plimit3         e_volume;
   Pstroke_data3   record;
   Wst_stroke_type type;
} Wst_defstroke;

typedef struct {
   Pfloat       value;
   Pint         num_pets;
   Pint         pets[WST_MAX_NUM_PETS];
   Plimit3      e_volume;
   Pval_data3   record;
   Wst_val_type type;
} Wst_defval;

typedef struct {
   Pint            choices;
   Pint            num_pets;
   Pint            pets[WST_MAX_NUM_PETS];
   Plimit3         e_volume;
   Pchoice_data3   record;
   Wst_choice_type type;
} Wst_defchoice;

typedef struct {
   Pint            max_bufsize;
   Pint            num_pets;
   Pint            pets[WST_MAX_NUM_PETS];
   Plimit3         e_volume;
   Pstring_data3   record;
   Wst_string_type type;
} Wst_defstring;

typedef struct {
   Pws_class           ws_class;
   Pdefer_mode         deferral_mode;
   Pmod_mode           modification_mode;

   /* Lines */
   Pint                num_predefined_polyline_indices;
   Pline_bundle_plus
      default_polyline_bundle_table[WST_MIN_PREDEF_LINE_REPS];

   /* Markers */
   Pint                num_predefined_polymarker_indices;
   Pmarker_bundle_plus
      default_polymarker_bundle_table[WST_MIN_PREDEF_MARKER_REPS];

   /* Text */
   Pint                num_predefined_text_indices;
   Ptext_bundle_plus
      default_text_bundle_table[WST_MIN_PREDEF_TEXT_REPS];

   /* Interiour */
   Pint                num_predefined_interior_indices;
   Pint_bundle_plus
      default_interior_bundle_table[WST_MIN_PREDEF_INTERIOR_REPS];

   /* Edge */
   Pint                num_predefined_edge_indices;
   Pedge_bundle_plus
      default_edge_bundle_table[WST_MIN_PREDEF_EDGE_REPS];

   /* Colour */
   Pint                default_colour_model;
   int                 has_double_buffer;
} Wst_output_wsdt;

typedef struct {
   Pnum_in       num_devs;
   Wst_defloc    locators[WST_MAX_NUM_LOCATOR_DEVS];
   Wst_defpick   picks[WST_MAX_NUM_PICK_DEVS];
   Wst_defstroke strokes[WST_MAX_NUM_STROKE_DEVS];
   Wst_defval     valuators[WST_MAX_NUM_VALUATOR_DEVS];
   Wst_defchoice choices[WST_MAX_NUM_CHOICE_DEVS];
   Wst_defstring  strings[WST_MAX_NUM_STRING_DEVS];
} Wst_input_wsdt;

struct _Ws;
struct _Phg_args_open_ws;

typedef struct {
   Pws_cat    ws_category;
   Pdc_units  dev_coord_units;
   Pfloat     dev_coords[3];
   Pint       dev_addrs_units[3];
   Pint       num_hlhsr_modes;
   Pint       *hlhsr_modes;

   /* View */
   Pint       num_predefined_views;
   Pview_rep3
      default_views[WST_MIN_PREDEF_VIEW_REPS];

   Wst_output_wsdt out_dt;
   Wst_input_wsdt  in_dt;

   struct _Ws* (*ws_open)(
                  struct _Phg_args_open_ws *args,
                  Phg_ret *ret
                  );
} Wst_phigs_dt;

typedef struct {
   int           x, y;
   unsigned int  width, height;
   unsigned int  border_width;
   unsigned long background;
   char          label[PHIGS_MAX_NAME_LEN + 1];
   char          icon_label[PHIGS_MAX_NAME_LEN + 1];
} Wst_xtool_dt;

typedef struct {
   Pint         num_pick_device_types;
   Pint         *pick_device_types;
   Wst_xtool_dt tool;
} Wst_xwin_dt;

typedef struct {
   Wst_phigs_dt    phigs_dt;
   Wst_xwin_dt     xwin_dt;
} Wst_dt;

typedef struct {
   Node             node;
   Wst_bound_status bound_status;
   Err_handle       erh;
   Pint             wsid;
   Pint             ws_type;
   Wst_dt           desc_tbl;
} Wst;

/*******************************************************************************
 * phg_wst_create
 *
 * DESCR:       Create workstation type structure
 * RETURNS:	Pointer to workstation type or NULL
 */

Wst* phg_wst_create(
   Err_handle erh,
   Pint ws_type
   );

/*******************************************************************************
 * phg_wst_init
 *
 * DESCR:       Initialize workstation type structure
 * RETURNS:     TRUE or FALSE
 */

int phg_wst_init(
   Wst *wst,
   Err_handle erh,
   Pint ws_type
   );

/*******************************************************************************
 * phg_wst_find
 *
 * DESCR:       Find workstation type
 * RETURNS:     Pointer to workstation type or NULL
 */

Wst* phg_wst_find(
   List *list,
   Pint ws_type
   );

/*******************************************************************************
 * phg_wst_destroy
 *
 * DESCR:       Destroy workstation type structure
 * RETURNS:     N/A
 */

void phg_wst_destroy(
   Wst *wst
   );

/*******************************************************************************
 * phg_wst_check_set_rep
 *
 * DESCR:       Check workstation attribute to set
 * RETURNS:     Pointer to Wst_phigs_dt structure or NULL
 */

Wst_phigs_dt* phg_wst_check_set_rep(
   Pint fn_id,
   Pint ws_id,
   Pint ind,
   Pint colr_ind
   );

/*******************************************************************************
 * phg_wst_add_ws_type
 *
 * DESCR:       Add workstation type
 * RETURNS:     TRUE or FALSE
 */

int phg_wst_add_ws_type(
   Pws_cat category,
   int double_buffer
   );

/*******************************************************************************
 * phg_wst_remove_ws_types
 *
 * DESCR:       Remove workstation types
 * RETURNS:     N/A
 */

void phg_wst_remove_ws_types(
   void
   );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ws_type_h */
