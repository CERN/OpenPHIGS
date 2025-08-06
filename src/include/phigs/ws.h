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

#ifndef _ws_h
#define _ws_h

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <GL/glx.h>

#include "phigs.h"
#include "ws_type.h"
#include "util.h"
#include "ws_inp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_DEFERRAL            5
#define NUM_MODIFICATION        3
#define NUM_SELECTABLE_STRUCTS  256
#define WS_MAX_NAMES_IN_NAMESET 1024
#define WS_MAX_LIGHT_SRC        8

/* hard code display size */
#define DISPLAY_WIDTH  1024
#define DISPLAY_HEIGHT 1024

/* bind attributes */
#define vCOLOR 1

typedef enum {
   PHG_TIME_NOW,
   PHG_TIME_BIG,
   PHG_TIME_BIL,
   PHG_TIME_ATI,
   PHG_TIME_NUM
} Ws_update_time;

typedef enum {
   PHG_UPDATE_ACCURATE,
   PHG_UPDATE_UWOR,
   PHG_UPDATE_UQUM,
   PHG_UPDATE_NOTHING,
   PHG_UPDATE_IF_IG,
   PHG_UPDATE_IF_IL,
   PHG_UPDATE_IF_INCORRECT,
   ASSURE_CORRECT,
   PHG_UPDATE_NUM
} Ws_update_action;

typedef enum {
   WS_PRE_CSS_DELETE,
   WS_POST_CSS_DELETE
} Ws_delete_flag;

typedef enum {
   WS_INV_NOT_CURRENT,
   WS_INV_CURRENT,
   WS_INT_NOT_INVERTIBLE
} Ws_inverse_state;

typedef struct {
   Pint x, y;
} Ws_point;

typedef struct {
   Node             node;
   Pint             id;
   Pint             priority;
   Pview_rep3       *viewrep;
   Ws_inverse_state npc_to_wc_state;
   Pmatrix3         npc_to_wc;
} Ws_view_ref;

typedef struct _Ws_post_str {
   Pfloat              disp_pri;
   Struct_handle       structh;
   struct _Ws_post_str *lower;
   struct _Ws_post_str *higher;
} Ws_post_str;

typedef struct {
   Ws_post_str lowest;
   Ws_post_str highest;
} Ws_posted_structs;

typedef Ws_update_action
   Ws_action_table[PHG_TIME_NUM][NUM_MODIFICATION][NUM_DEFERRAL];

typedef Ws_action_table *Ws_action_table_ptr;

typedef struct {
   Ppoint3 scale;
   Ppoint3 offset;
} Ws_xform;

typedef struct {
   int unused;
} Wsa_output_ws;

typedef struct {
   Css_handle          cssh;

   Pvisual_st          vis_rep;
   Pdisp_surf_empty    surf_state;

   /* Window */
   Plimit3             ws_window;
   Plimit3             ws_viewport;
   Pupd_st             ws_window_pending;
   Pupd_st             ws_viewport_pending;
   Plimit3             req_ws_window;
   Plimit3             req_ws_viewport;
   Ws_xform            ws_xform;

   /* Views */
   Pupd_st             views_pending;
   List                pending_views;
   List                views;

   /* Structures */
   Ws_posted_structs   posted;

   Ws_update_action    now_action;
   Ws_action_table_ptr update_action_table;

   /* Render modes */
   Pupd_st             hlhsr_mode_pending;
   Pint                req_hlhsr_mode;
   Pint                cur_hlhsr_mode;
} Wsb_output_ws;

typedef struct {
   Pdefer_mode      def_mode;
   Pmod_mode        mod_mode;

   struct {
      Hash_table    marker;
      Hash_table    line;
      Hash_table    text;
      Hash_table    interior;
      Hash_table    edge;
      Hash_table    colour;
      Hash_table    view;
      Hash_table    light_source;
   } htab;

   struct {
      Nameset       invis_incl;
      Nameset       invis_excl;
   } nset;

   struct {
      Nameset       high_incl;
      Nameset       high_excl;
   } hnset;

   union {
      Wsa_output_ws a;
      Wsb_output_ws b;
   } model;
} Ws_output_ws;

struct _Wsgl;
typedef struct _Wsgl *Wsgl_handle;

typedef struct _Ws {
   Pint         id;
   Wst          *type;
   Pws_cat      category;
   Ws_output_ws out_ws;
   Ws_input_ws  in_ws;
   Pint         current_colour_model;
   Pint         num_active_input_devs;

   Err_handle   erh;
   Phg_scratch  scratch;

   /* Window system variables */
   Display      *display;
   Window       drawable_id;
   GLXContext   glx_context;
   XtAppContext app_context;
   Window       input_overlay_window;
   Wsgl_handle  render_context;
   int          has_double_buffer;
   XRectangle   ws_rect;
   Widget       top_level; /* only in PM */
   Widget       msg_shell; /* only in PM */
   Widget       msg_label; /* only in PM */
   Widget       shell;
   Pint         num_boxed_valuators;
   Widget       valuator_shell;
   Widget       valuator_box;
   Widget       valuator_frame;
   GLXFBConfig  *fbc;
   GLuint       fbuf, depthbuf, colorbuf;
   GLint        old_viewport[4];
   GLint        program;

   /* Output LUN for some work station types, e.g. to print out stuff here */
   Pint         lun;
   Pfloat       hcsf;
   /* Output File name and descriptor*/
   char         filename[512];
   FILE         *fd;
   void         (*close)(
                   struct _Ws *ws
                   );
   void         (*redraw_all)(
                   struct _Ws *ws,
                   Pctrl_flag clear_control
                   );
   void         (*conditional_redraw)(
                   struct _Ws *ws
                   );
   void         (*repaint_all)(
                   struct _Ws *ws,
                   Pctrl_flag clear_control
                   );
   void         (*make_requested_current)(
                   struct _Ws *ws
                   );
   void         (*update)(
                   struct _Ws *ws,
                   Pregen_flag flag
                   );
   void         (*set_disp_update_state)(
                   struct _Ws *ws,
                   Pdefer_mode def_mode,
                   Pmod_mode mod_mode
                   );
   void         (*message)(
                   struct _Ws *ws,
                   Phg_args_message *args
                   );
   void         (*set_hlhsr_mode)(
                   struct _Ws *ws,
                   Pint mode
                   );
   void         (*set_rep)(
                   struct _Ws *ws,
                   Phg_args_rep_type type,
                   Phg_args_rep_data *rep);
   void         (*set_filter)(
                   struct _Ws *ws,
                   Phg_args_flt_type type,
                   Pint dev_id,
                   Pint_list *incl_set,
                   Pint_list *excl_set
                   );
   void         (*set_ws_window)(
                   struct _Ws *ws,
                   Pint two_d,
                   Plimit3 *limits
                   );
   void         (*set_ws_vp)(
                   struct _Ws *ws,
                   Pint two_d,
                   Plimit3 *limits
                   );
   void         (*add_el)(
                   struct _Ws *ws
                   );
   void         (*copy_struct)(
                   struct _Ws *ws,
                   El_handle first_el
                   );
   void         (*close_struct)(
                   struct _Ws *ws,
                   Struct_handle structh
                   );
   void         (*move_ep)(
                   struct _Ws *ws,
                   El_handle ep
                );
   int          (*delete_el)(
                   struct _Ws *ws,
                   Struct_handle structh,
                   El_handle elh1,
                   El_handle elh2,
                   Ws_delete_flag flag
                   );
   int          (*delete_struct)(
                   struct _Ws *ws,
                   Struct_handle structh,
                   Ws_delete_flag flag
                   );
   int          (*delete_struct_net)(
                   struct _Ws *ws,
                   Struct_handle structh,
                   Pref_flag reff,
                   Ws_delete_flag flag
                   );
   void         (*delete_all_structs) (
                   struct _Ws *ws
                   );
   void         (*post)(
                   struct _Ws *ws,
                   Struct_handle structh,
                   Pfloat priority,
                   int first_posting
                   );
   void         (*unpost)(
                    struct _Ws *ws,
                    Struct_handle structh
                    );
   void         (*unpost_all)(
                    struct _Ws *ws
                    );
   void         (*change_posting)(
                   struct _Ws *ws,
                   Struct_handle unpost,
                   Struct_handle post
                   );
   void         (*inq_posted)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*inq_disp_update_state)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*inq_filter)(
                   struct _Ws *ws,
                   Phg_args_flt_type type,
                   Phg_ret *ret
                   );
   void         (*inq_hlhsr_mode)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*inq_representation)(
                   struct _Ws *ws,
                   Pint index,
                   Pinq_type how,
                   Phg_args_rep_type rep_type,
                   Phg_ret *ret
                   );
    void        (*inq_view_indices)(
                   struct _Ws *ws,
                   Phg_ret *ret
                   );
   void         (*inq_bundle_indices)(
                   struct _Ws *ws,
                   Phg_args_rep_type rep_type,
                   Phg_ret *ret
                );
   void         (*set_view_input_priority)(
                   struct _Ws *ws,
                   Pint index,
                   Pint ref_index,
                   Prel_pri priority
                   );
   int          (*map_initial_points)(
                   struct _Ws *ws,
                   Pint view_index,
                   Pint *num_pts,
                   Ppoint3 *wc_pts,
                   XPoint *dwbl_pts
                   );
   int          (*resolve_locator)(
                   struct _Ws *ws,
                   Ws_point *dc_pt,
                   int determine_z,
                   Pint *view_index,
                   Ppoint3 *wc_pt
                   );
   int          (*point_in_viewport)(
                   struct _Ws *ws,
                   XPoint *pt
                   );
   int         (*resolve_stroke)(
                   struct _Ws *ws,
                   Pint num_pts,
                   Ws_point *dc_pts,
                   int determine_z,
                   Pint *view_index,
                   Ppoint_list3 *wc_pts
                   );
   int          (*resolve_pick)(
                   struct _Ws *ws,
                   Ws_inp_pick *dev,
                   int echo,
                   Ws_point *dc_pt,
                   Ppick *pick
                   );

   /* Not used by all workstations */
   int          (*valid_pick_path)(
                   struct _Ws *ws,
                   Ppick *pick
                   );
   int          (*pick_enable)(
                   struct _Ws *ws,
                   Ws_inp_pick *dev
                   );
   void         (*pick_disable)(
                   struct _Ws *ws,
                   Ws_inp_pick *dev
                   );

   /* Initialized by input module */
   void         (*init_device)(
                   struct _Ws *ws,
                   Phg_args_inp_init_dev *args
                   );
   void         (*set_device_mode)(
                   struct _Ws *ws,
                   Phg_args_set_mode_data *args
                   );
   void         (*request_device)(
                   struct _Ws *ws,
                   Phg_args_idev_class idev_class,
                   Pint dev_num,
                   Phg_ret *ret
                   );
   void         (*sample_device)(
                   struct _Ws *ws,
                   Phg_args_idev_class idev_class,
                   Pint dev_num,
                   Phg_ret *ret
                   );
   void         (*input_repaint)(
                   struct _Ws *ws,
                   Pint num_rects,
                   XRectangle *rects
                   );
   void         (*inq_inp_dev_state)(
                   struct _Ws *ws,
                   Phg_args_idev_class idev_class,
                   Pint num,
                   Phg_ret *ret
                   );
} Ws;

#define case_PHG_UPDATE_ACCURATE_or_IF_Ix	\
   case PHG_UPDATE_ACCURATE:			\
   case PHG_UPDATE_IF_IG:			\
   case PHG_UPDATE_IF_IL

#define WS_SET_WS_RECT(_wsh, _wattr)		\
{						\
   (_wsh)->ws_rect.x = (_wattr)->x;		\
   (_wsh)->ws_rect.y = (_wattr)->y;		\
   (_wsh)->ws_rect.width = (_wattr)->width;	\
   (_wsh)->ws_rect.height = (_wattr)->height;	\
}

#define WS_ANY_INP_DEV_ACTIVE(_wsh) \
   ((_wsh)->num_active_input_devs > 0)

#define WS_DRWBL_TO_DC2( _wsh, _dwp, _dcp ) \
    ((_dcp)->x = (_dwp)->x, \
     (_dcp)->y = (_wsh)->ws_rect.height - (_dwp)->y)

#define WS_NPC_TO_DC(_wsxf, _npc, _dc) \
    (_dc)->x = (_npc)->x * (_wsxf)->scale.x + (_wsxf)->offset.x; \
    (_dc)->y = (_npc)->y * (_wsxf)->scale.y + (_wsxf)->offset.y; \
    (_dc)->z = (_npc)->z * (_wsxf)->scale.z + (_wsxf)->offset.z;

#define WS_DC_TO_DRWBL2( _wsh, _dcp, _dwp ) \
    ((_dwp)->x = (_dcp)->x, \
     (_dwp)->y = (_wsh)->ws_rect.height - (_dcp)->y)

#define WS_DC_TO_NPC2(_wsxf, _dc, _npc) \
    (_npc)->x = ( (_dc)->x - (_wsxf)->offset.x) / (_wsxf)->scale.x; \
    (_npc)->y = ( (_dc)->y - (_wsxf)->offset.y) / (_wsxf)->scale.y;

#define WS_DC_TO_NPC(_wsxf, _dc, _npc) \
    (_npc)->x = ( (_dc)->x - (_wsxf)->offset.x) / (_wsxf)->scale.x; \
    (_npc)->y = ( (_dc)->y - (_wsxf)->offset.y) / (_wsxf)->scale.y; \
    (_npc)->z = ( (_dc)->z - (_wsxf)->offset.z) / (_wsxf)->scale.z;

#define WS_PT_IN_LIMIT2( lim, pt) \
    (  (pt)->x >= (lim)->x_min && (pt)->x <= (lim)->x_max \
    && (pt)->y >= (lim)->y_min && (pt)->y <= (lim)->y_max)

#define WS_PT_IN_LIMIT( lim, pt) \
    (  (pt)->x >= (lim)->x_min && (pt)->x <= (lim)->x_max \
    && (pt)->y >= (lim)->y_min && (pt)->y <= (lim)->y_max \
    && (pt)->z >= (lim)->z_min && (pt)->z <= (lim)->z_max)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ws_h */
