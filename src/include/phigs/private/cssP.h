/******************************************************************************

Copyright (c) 1989,1990, 1991  X Consortium

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

#ifndef _cssP_h
#define _cssP_h

#ifdef __cplusplus
extern "C" {
#endif

#define CSS_MEM_BLOCKSIZE	1024

/* opcodes for handling structure element data */
typedef enum {
    CSS_EL_CREATE,
    CSS_EL_REPLACE,
    CSS_EL_COPY,
    CSS_EL_AR_TO_CSS,
    CSS_EL_CSS_TO_AR,
    CSS_EL_FREE,
    CSS_EL_INQ_CONTENT,
    CSS_EL_INQ_TYPE_SIZE
} Css_el_op;

typedef enum {
    CSS_WS_APPEAR,
    CSS_WS_POST,
    CSS_WS_BOTH
} Css_ws_list_op;

#define CSS_MEM_BLOCK(cssh, nbytes, blockptr, blocktype)		\
  { unsigned _size = (nbytes);\
									\
    if (_size <= CSS_MEM_BLOCKSIZE) _size = CSS_MEM_BLOCKSIZE;		\
    if (!(cssh)->mem) {							\
	(cssh)->mem = (caddr_t)malloc(_size);				\
	if ((cssh)->mem)						\
	    (cssh)->mem_size = _size;					\
    } else {								\
	/* grow or shrink the memory block if necessary */		\
	if (_size > (cssh)->mem_size || (_size<=CSS_MEM_BLOCKSIZE &&	\
				       (cssh)->mem_size>CSS_MEM_BLOCKSIZE)) { \
	    free((cssh)->mem);						\
	    (cssh)->mem = (caddr_t)malloc(_size);			\
	    if ((cssh)->mem)						\
		(cssh)->mem_size = _size;				\
	}								\
    }									\
    (blockptr) = (blocktype *)((cssh)->mem);				\
  }

#define CSS_CREATE_EL(cssh, elptr)					\
    if ( !((elptr) = (El_handle) malloc(sizeof(Css_structel))) ) {	\
	ERR_BUF((cssh)->erh, ERR900);					\
	return(FALSE);                              /* out of memory */	\
    }

#define CSS_INSERT_EL(cssh, elptr)					\
        (elptr)->next = (cssh)->el_ptr->next;	/* insert new element */ \
        (elptr)->next->prev = (elptr);					\
        (cssh)->el_ptr->next = (elptr);					\
        (elptr)->prev = (cssh)->el_ptr;					\
        (cssh)->el_ptr = (elptr);	/* make new element the current one */ \
        (cssh)->open_struct->num_el++;					\
        (cssh)->el_index++;

#define CSS_UPDATE_EL_INDEX(cssh)					\
  { register El_handle _elptr;						\
									\
    (cssh)->el_index = 0;						\
    _elptr = (cssh)->el_ptr;						\
    while (_elptr->prev) {						\
	(cssh)->el_index++;						\
	_elptr = _elptr->prev;						\
    }									\
  }

#define CSS_EMPTY_STRUCT(cssh, structid)				\
  { El_handle			_ep1, _ep2;				\
    Phg_args_del_el_data	_data;					\
									\
    _data.struct_id = (structid);					\
    phg_css_el_delete_list((cssh), PHG_ARGS_EMPTY_STRUCT, &_data, &_ep1,&_ep2);\
    if (_ep1 && _ep2)							\
	phg_css_delete_el((cssh), PHG_ARGS_EMPTY_STRUCT, &_data, _ep1, _ep2); }

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _cssP_h */

