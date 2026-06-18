/******************************************************************************

Copyright (c) 1989,1990, 1991  X Consortium
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

Copyright (c) 1989,1990, 1991 by Sun Microsystems, Inc.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Sun Microsystems,
and the X Consortium, not be used in advertising or publicity
pertaining to distribution of the software without specific, written
prior permission.

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "phg.h"
#include "css.h"
#include "private/cssP.h"
#include "private/phgP.h"

static void css_print_refer_to_me(Css_set_ptr s);
static void css_print_i_refer_to(Css_set_ptr s);

/*******************

    phg_css_print_struct - print contents of given structure

*******************/

void phg_css_print_struct(Struct_handle structp, int arflag)
{
  El_handle    elptr;
  Css_ws_list    wsptr;
  int      i = 1;

  if (!structp) {
    fprintf(stderr, "*** structp is NULL ***\n\n");
    return;
  }
  fprintf(stderr, "structure id: %d\n", structp->struct_id);
  fprintf(stderr, "workstations posted to: ");
  if ( !(wsptr = structp->ws_posted_to) )
    fprintf(stderr, "none");
  else
    while (wsptr->wsh) {
      fprintf(stderr, "%d(x%d) ", wsptr->wsh->id, wsptr->count);
      wsptr++;
    }
  fprintf(stderr, "\nworkstations appearing on: ");
  if ( !(wsptr = structp->ws_appear_on) )
    fprintf(stderr, "none");
  else
    while (wsptr->wsh) {
      fprintf(stderr, "%d(x%d) ", wsptr->wsh->id, wsptr->count);
      wsptr++;
    }
  fprintf(stderr, "\n");
  css_print_refer_to_me(structp->refer_to_me);
  css_print_i_refer_to(structp->i_refer_to);
  fprintf(stderr, "number of elements: %d\n\n", structp->num_el);
  elptr = structp->first_el->next;
  while (elptr != structp->last_el) {
    fprintf(stderr, "%d. ", i++);
    phg_css_print_eldata(elptr, arflag);
    elptr = elptr->next;
  }
  fprintf(stderr, "---------------------\n");
}

/*******************

    phg_css_print_eldata - print contents of given structure element

*******************/

void phg_css_print_eldata(El_handle elptr, int arflag)
{
  Struct_handle structp;
  if (!elptr) {
    fprintf(stderr, "*** elptr is NULL ***\n");
    return;
  }
  css_print_eltype(elptr->eltype);

  switch(elptr->eltype) {
    /* TODO: Print the element data. */
  case PELEM_HLHSR_ID:
  case PELEM_INT_IND:
  case PELEM_EDGE_IND:
  case PELEM_LINETYPE:
  case PELEM_VIEW_IND:
  case PELEM_LABEL:
  case PELEM_INT_STYLE:
  case PELEM_EDGE_COLR_IND:
  case PELEM_INT_COLR_IND:
  case PELEM_INT_SHAD_METH:
  case PELEM_INT_REFL_EQN:
  case PELEM_MODEL_CLIP_IND:
    fprintf(stderr, "%d", PHG_INT(elptr));
  case PELEM_ALPHA_CHANNEL:
    fprintf(stderr, "%f", PHG_FLOAT(elptr));
  case PELEM_NIL:
  default:
    /* no data */
    fprintf(stderr, "\n");
    break;
  }
  fprintf(stderr, "\n");
  fflush(stdout);
}

/*******************

    css_print_eltype - print element type of enum Peltype variable

*******************/

void css_print_eltype(Pelem_type eltype)
{
  char  *name = NULL;

  switch(eltype) {
  case PELEM_NIL: name = "PELEM_NIL"; break;
  case PELEM_POLYLINE3: name = "PELEM_POLYLINE3"; break;
  case PELEM_POLYLINE: name = "PELEM_POLYLINE"; break;
  case PELEM_POLYMARKER3: name = "PELEM_POLYMARKER3"; break;
  case PELEM_POLYMARKER: name = "PELEM_POLYMARKER"; break;
  case PELEM_TEXT: name = "PELEM_TEXT"; break;
  case PELEM_TEXT3: name = "PELEM_TEXT3"; break;
  case PELEM_FILL_AREA3: name = "PELEM_FILL_AREA3"; break;
  case PELEM_FILL_AREA_SET: name = "PELEM_FILL_AREA_SET"; break;
  case PELEM_FILL_AREA_SET3: name = "PELEM_FILL_AREA_SET3"; break;
  case PELEM_FILL_AREA_SET_DATA:
    name = "PELEM_FILL_AREA_SET_DATA";
    break;
  case PELEM_FILL_AREA_SET3_DATA:
    name = "PELEM_FILL_AREA_SET3_DATA";
    break;
  case PELEM_SET_OF_FILL_AREA_SET3_DATA:
    name = "PELEM_FILL_AREA_SET3_DATA";
    break;
  case PELEM_FILL_AREA: name = "PELEM_FILL_AREA"; break;
  case PELEM_LINE_IND: name = "PELEM_LINE_IND"; break;
  case PELEM_LINETYPE: name = "PELEM_LINETYPE"; break;
  case PELEM_LINEWIDTH: name = "PELEM_LINEWIDTH"; break;
  case PELEM_LINE_COLR_IND: name = "PELEM_LINE_COLR_IND"; break;
  case PELEM_MARKER_IND: name = "PELEM_MARKER_IND"; break;
  case PELEM_MARKER_TYPE: name = "PELEM_MARKER_TYPE"; break;
  case PELEM_MARKER_SIZE: name = "PELEM_MARKER_SIZE"; break;
  case PELEM_MARKER_COLR_IND: name = "PELEM_MARKER_COLR_IND"; break;
  case PELEM_TEXT_IND: name = "PELEM_TEXT_IND"; break;
  case PELEM_TEXT_FONT: name = "PELEM_TEXT_FONT"; break;
  case PELEM_TEXT_PREC: name = "PELEM_TEXT_PREC"; break;
  case PELEM_TEXT_PATH: name = "PELEM_TEXT_PATH"; break;
  case PELEM_TEXT_ALIGN: name = "PELEM_TEXT_ALIGN"; break;
  case PELEM_CHAR_HT: name = "PELEM_CHAR_HT"; break;
  case PELEM_CHAR_EXPAN: name = "PELEM_CHAR_EXPAN"; break;
  case PELEM_CHAR_SPACE: name = "PELEM_CHAR_SPACE"; break;
  case PELEM_CHAR_UP_VEC: name = "PELEM_CHAR_UP_VEC"; break;
  case PELEM_TEXT_COLR_IND: name = "PELEM_TEXT_COLR_IND"; break;
  case PELEM_INT_IND: name = "PELEM_INT_IND"; break;
  case PELEM_INT_STYLE: name = "PELEM_INT_STYLE"; break;
  case PELEM_BACK_INT_STYLE: name = "PELEM_BACK_INT_STYLE"; break;
  case PELEM_INT_STYLE_IND: name = "PELEM_INT_STYLE_IND"; break;
  case PELEM_BACK_INT_STYLE_IND: name = "PELEM_BACK_INT_STYLE_IND"; break;
  case PELEM_INT_COLR_IND: name = "PELEM_INT_COLR_IND"; break;
  case PELEM_EDGE_IND: name = "PELEM_EDGE_IND"; break;
  case PELEM_EDGE_FLAG: name = "PELEM_EDGE_FLAG"; break;
  case PELEM_EDGETYPE: name = "PELEM_EDGETYPE"; break;
  case PELEM_EDGEWIDTH: name = "PELEM_EDGEWIDTH"; break;
  case PELEM_EDGE_COLR_IND: name = "PELEM_EDGE_COLR_IND"; break;
  case PELEM_ADD_NAMES_SET: name = "PELEM_ADD_NAMES_SET"; break;
  case PELEM_REMOVE_NAMES_SET: name = "PELEM_REMOVE_NAMES_SET"; break;
  case PELEM_INDIV_ASF: name = "PELEM_INDIV_ASF"; break;
  case PELEM_LOCAL_MODEL_TRAN3: name = "PELEM_LOCAL_MODEL_TRAN3"; break;
  case PELEM_GLOBAL_MODEL_TRAN3: name = "PELEM_GLOBAL_MODEL_TRAN3"; break;
  case PELEM_VIEW_IND: name = "PELEM_VIEW_IND"; break;
  case PELEM_EXEC_STRUCT: name = "PELEM_EXEC_STRUCT"; break;
  case PELEM_LABEL: name = "PELEM_LABEL"; break;
  case PELEM_PICK_ID: name = "PELEM_PICK_ID"; break;
  case PELEM_HLHSR_ID: name = "PELEM_HLHSR_ID"; break;
  case PELEM_INT_COLR: name = "PELEM_INT_COLR"; break;
  case PELEM_BACK_INT_COLR: name = "PELEM_BACK_INT_COLR"; break;
  case PELEM_LINE_COLR: name = "PELEM_LINE_COLR"; break;
  case PELEM_MARKER_COLR: name = "PELEM_MARKER_COLR"; break;
  case PELEM_EDGE_COLR: name = "PELEM_EDGE_COLR"; break;
  case PELEM_TEXT_COLR: name = "PELEM_TEXT_COLR"; break;
  case PELEM_LIGHT_SRC_STATE: name = "PELEM_LIGHT_SRC_STATE"; break;
  case PELEM_INT_SHAD_METH: name = "PELEM_INT_SHAD_METH"; break;
  case PELEM_BACK_INT_SHAD_METH: name = "PELEM_BACK_INT_SHAD_METH"; break;
  case PELEM_INT_REFL_EQN: name = "PELEM_INT_REFL_EQN"; break;
  case PELEM_BACK_INT_REFL_EQN: name = "PELEM_BACK_INT_REFL_EQN"; break;
  case PELEM_REFL_PROPS: name = "PELEM_REFL_PROP"; break;
  case PELEM_BACK_REFL_PROPS: name = "PELEM_BACK_REFL_PROP"; break;
  case PELEM_FACE_DISTING_MODE: name = "PELEM_FACE_DISTING_MODE"; break;
  case PELEM_FACE_CULL_MODE: name = "PELEM_FACE_CULL_MODE"; break;
  case PELEM_ANNO_TEXT_REL3: name = "PELEM_ANNO_TEXT_REL3"; break;
  case PELEM_ANNO_TEXT_REL: name = "PELEM_ANNO_TEXT_REL"; break;
  case PELEM_ANNO_ALIGN: name = "PELEM_ANNO_ALIGN"; break;
  case PELEM_ANNO_CHAR_HT: name = "PELEM_ANNO_CHAR_HT"; break;
  case PELEM_MODEL_CLIP_VOL3: name = "PELEM_MODEL_CLIP_VOL3"; break;
  case PELEM_MODEL_CLIP_IND: name = "PELEM_MODEL_CLIP_IND"; break;
  case PELEM_GSE: name = "PELEM_GSE"; break;
  case PELEM_ALPHA_CHANNEL: name = "PELEM_ALPHA_CHANNEL"; break;
  case PELEM_INT_REFL_MODEL: name = "PELEM_INT_REFL_MODEL"; break;

  default:
    fprintf(stderr, "UNKNOWN TYPE: %d\n", eltype);
    break;
  }

  if ( name )
    fprintf( stderr, "%s\n", name );
}

/*******************

    css_print_refer_to_me - print list of structures that refer to this one

*******************/

static void css_print_refer_to_me(Css_set_ptr s)
{
  Css_set_element *el;

  fprintf(stderr, "structures that refer to me: ");
  if ( !(el = s->elements->next) )
    fprintf(stderr, "none");
  else {
    while (el) {
      fprintf(stderr, "%d(x%d) ",
              ((Struct_handle)el->key)->struct_id, (unsigned)((long)el->data));
      el = el->next;
    }
  }
  fprintf(stderr, "\n");
}

/*******************

    css_print_i_refer_to - print list of structures this one executes

*******************/

static void css_print_i_refer_to(Css_set_ptr s)
{
  Css_set_element   *el1, *el2;
  int      eindex, i, n;

  fprintf(stderr, "structures i execute: ");
  if ( !(el1 = s->elements->next) )
    fprintf(stderr, "none");
  else {
    while (el1) {
      fprintf(stderr, "%d(", ((Struct_handle)el1->key)->struct_id);
      n = ((Css_set_ptr)el1->data)->num_elements;
      i = 1;
      el2 = ((Css_set_ptr)el1->data)->elements->next;
      while (el2) {
        /* get index of execute structure element */
        CSS_GET_EL_INDEX((El_handle)el2->key, eindex)
          fprintf(stderr, (i < n ? "%d, " : "%d"), eindex);
        el2 = el2->next;
        i++;
      }
      fprintf(stderr, ") ");
      el1 = el1->next;
    }
  }
  fprintf(stderr, "\n");
}
