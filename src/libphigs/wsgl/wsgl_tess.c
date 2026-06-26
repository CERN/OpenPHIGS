#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "private/wsglP.h"
#include "private/wsgl_tessP.h"

#ifndef CALLBACK
#if defined(_WIN32)
#define CALLBACK __stdcall
#else
#define CALLBACK
#endif
#endif

static void CALLBACK tessBeginCB(GLenum which) {
    glBegin(which);
}

static void CALLBACK tessEndCB() {
    glEnd();
}

static void CALLBACK tessVertexCB(void *data) {
    Wsgl_tess_vertex *v = (Wsgl_tess_vertex *)data;
    if (v->apply_cb) {
        v->apply_cb(v);
    }
    glVertex3dv(v->pos);
}

static void CALLBACK tessCombineCB(GLdouble coords[3], 
                                   void *vertex_data[4], 
                                   GLfloat weight[4], 
                                   void **outData) {
    /* Basic combine callback to prevent crashing on self-intersecting polygons */
    Wsgl_tess_vertex *new_vert = (Wsgl_tess_vertex *)malloc(sizeof(Wsgl_tess_vertex));
    if (new_vert) {
        memset(new_vert, 0, sizeof(Wsgl_tess_vertex));
        new_vert->pos[0] = coords[0];
        new_vert->pos[1] = coords[1];
        new_vert->pos[2] = coords[2];
        if (vertex_data[0]) {
            Wsgl_tess_vertex *v0 = (Wsgl_tess_vertex *)vertex_data[0];
            new_vert->apply_cb = v0->apply_cb;
            new_vert->ws = v0->ws;
            new_vert->colr_type = v0->colr_type;
            new_vert->colr = v0->colr;
            new_vert->ast = v0->ast;
            new_vert->has_norm = v0->has_norm;
            if (new_vert->has_norm) {
                new_vert->norm[0] = v0->norm[0];
                new_vert->norm[1] = v0->norm[1];
                new_vert->norm[2] = v0->norm[2];
            }
        }
        *outData = new_vert;
    } else {
        *outData = vertex_data[0];
    }
}

static void CALLBACK tessErrorCB(GLenum errorCode) {
    const GLubyte *errorStr;
    errorStr = gluErrorString(errorCode);
    /* fprintf(stderr, "OpenPHIGS GLU Tessellation Error: %s\n", errorStr); */
}

void wsgl_draw_tess_polygon(Wsgl_tess_vertex *vertices, int num_vertices, int record_geom_flag)
{
    GLUtesselator *tess;
    int i;
    int vertex_indices[MAX_VERTICES];
    int normal_indices[MAX_VERTICES];
    int n_vertices = 0;
    int n_normals = 0;

    tess = gluNewTess();
    if (!tess) return;

    gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK *)())tessBeginCB);
    gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK *)())tessEndCB);
    gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK *)())tessVertexCB);
    gluTessCallback(tess, GLU_TESS_ERROR, (void (CALLBACK *)())tessErrorCB);
    gluTessCallback(tess, GLU_TESS_COMBINE, (void (CALLBACK *)())tessCombineCB);

    gluTessBeginPolygon(tess, NULL);
    gluTessBeginContour(tess);

    for (i = 0; i < num_vertices; i++) {
        gluTessVertex(tess, vertices[i].pos, (void *)&vertices[i]);

        if (record_geom_flag) {
            vertex_indices[n_vertices] = wsgl_add_vertex((float)vertices[i].pos[0],
                                                         (float)vertices[i].pos[1],
                                                         (float)vertices[i].pos[2]);
            n_vertices++;
            normal_indices[n_normals] = wsgl_add_normal(current_normal.x,
                                                        current_normal.y,
                                                        current_normal.z);
            n_normals++;
        }
    }

    gluTessEndContour(tess);
    gluTessEndPolygon(tess);
    gluDeleteTess(tess);

    if (record_geom_flag && n_vertices > 0) {
        wsgl_add_geometry(GEOM_FACE, vertex_indices, normal_indices, n_vertices);
    }
}
