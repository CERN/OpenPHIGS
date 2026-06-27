#ifndef _wsgl_tessP_h
#define _wsgl_tessP_h

#include <GL/gl.h>
#include <GL/glu.h>
#include "phg.h"

typedef struct {
    GLdouble pos[3];

    /* Callback for applying per-vertex attributes (like color/normal) */
    void (*apply_cb)(void *data);
    void *cb_data;
    
    /* Variables to avoid dynamic allocation for cb_data in simple cases */
    Ws *ws;
    Pint colr_type;
    Pcoval colr;
    Ws_attr_st *ast;
    
    int has_norm;
    Pfloat norm[3];
    
} Wsgl_tess_vertex;

void wsgl_draw_tess_polygon(Wsgl_tess_vertex *vertices, int num_vertices, int record_geom_flag);

#endif
