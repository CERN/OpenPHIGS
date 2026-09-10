/*
 * Draws THREE overlapping opaque fills via pfill_area_set_data() (PFASD),
 * all at Z=0, one after another -- green, then blue, then red -- the same
 * "later draws win the tie" scenario tie_break_test.c already validated for
 * plain pfill_area(), but through the PFASD code path the DELPHI dolphin
 * logo (delgrasubs.f's KYDELP) actually uses.
 *
 * PASS: centre pixel reads red (drawn last). FAIL: anything else.
 *
 * Build/run: see tie_break_test.c in this directory for the exact command.
 */
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#ifdef GLEW
#include <GL/glew.h>
#else
#include <epoxy/gl.h>
#endif

#include "phg.h"

static void draw_quad(Pgcolr * colr)
{
  Ppoint3 verts[4] = {
    {0.2, 0.2, 0.0}, {0.8, 0.2, 0.0}, {0.8, 0.8, 0.0}, {0.2, 0.8, 0.0}
  };
  Pfacet_data3 fdata;
  Pfacet_vdata_list3 vdata;

  pset_int_colr(colr);
  pset_back_int_colr(colr);
  vdata.num_vertices = 4;
  vdata.vertex_data.points = verts;
  pfill_area_set_data(PFACET_NONE, PEDGE_NONE, PVERT_COORD, PMODEL_RGB,
                      &fdata, 1, NULL, &vdata);
}

int main(void)
{
  Pgcolr green, blue, red;
  GLubyte pixel[3];
  int width, height;

  green.type = PMODEL_RGB;
  green.val.general.x = 0.0; green.val.general.y = 1.0; green.val.general.z = 0.0;
  blue.type = PMODEL_RGB;
  blue.val.general.x = 0.0; blue.val.general.y = 0.0; blue.val.general.z = 1.0;
  red.type = PMODEL_RGB;
  red.val.general.x = 1.0; red.val.general.y = 0.0; red.val.general.z = 0.0;

  popen_phigs(NULL, 0);
  popen_struct(1);
  pset_int_style(PSTYLE_SOLID);
  draw_quad(&green);  /* drawn first */
  draw_quad(&blue);   /* drawn second, fully overlapping */
  draw_quad(&red);    /* drawn last, fully overlapping -- should end up on top */
  pclose_struct();

  popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
  ppost_struct(0, 1, 0.0);
  pupd_ws(0, PFLAG_PERFORM);

  width = PHG_WSID(0)->ws_rect.width;
  height = PHG_WSID(0)->ws_rect.height;
  glReadBuffer(GL_FRONT); /* pupd_ws() already swapped */
  glReadPixels(width / 2, height / 2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

  printf("Centre pixel (3 overlapping PFASD fills, Z=0) = (%d, %d, %d)\n",
         pixel[0], pixel[1], pixel[2]);
  if (pixel[0] > pixel[1] && pixel[0] > pixel[2]){
    printf("PASS: red (drawn last) is on top.\n");
  } else {
    printf("FAIL: red is not on top (green/blue showing, or fill missing).\n");
  }

  pclose_ws(0);
  pclose_phigs();
  return 0;
}
