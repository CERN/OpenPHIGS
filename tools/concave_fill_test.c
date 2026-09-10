/*
 * Checks whether a CONCAVE filled polygon (which OpenPHIGS tessellates into
 * several triangles via GLU, see wsgl_tess.c/wsgl_tessP.h) renders correctly
 * under order independent rendering, as opposed to the single convex quad
 * tie_break_test.c already validated. Motivated by the DELPHI dolphin logo
 * (delgrasubs.f's KYDELP, drawn with PFASD, a fill-area-set-with-data path
 * that also tessellates) losing its fill (outline only) once the
 * sortFragments() depth-tie fix landed.
 *
 * Draws a single opaque GREEN 5-point star (concave: one reflex vertex) at
 * Z=0 and reports the colour at its centre, which should be green if the
 * fill is actually rasterised and survives to the final composite.
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

int main(void)
{
  /* a concave 6-point "arrow" shape: reflex vertex at (0.5, 0.5) */
  Ppoint pts[6] = {
    {0.2, 0.2}, {0.8, 0.2}, {0.8, 0.8}, {0.5, 0.5}, {0.2, 0.8}, {0.2, 0.2}
  };
  Ppoint_list list = { 6, pts };
  Pgcolr green;
  GLubyte pixel[3];
  int width, height;

  green.type = PMODEL_RGB;
  green.val.general.x = 0.0; green.val.general.y = 1.0; green.val.general.z = 0.0;

  popen_phigs(NULL, 0);
  popen_struct(1);
  pset_int_style(PSTYLE_SOLID);
  pset_int_colr(&green);
  pset_back_int_colr(&green);
  pfill_area(&list);
  pclose_struct();

  popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
  ppost_struct(0, 1, 0.0);
  pupd_ws(0, PFLAG_PERFORM);

  width = PHG_WSID(0)->ws_rect.width;
  height = PHG_WSID(0)->ws_rect.height;
  /* pupd_ws() already swapped buffers, so the rendered frame is now in
     front, not back -- reading GL_BACK here would see invalidated data */
  glReadBuffer(GL_FRONT);
  /* (0.3, 0.3) in NDF: inside the "wing" of the arrow, away from the
     reflex vertex, definitely inside the polygon regardless of winding */
  glReadPixels((int) (0.3 * width), (int) (0.3 * height), 1, 1,
               GL_RGB, GL_UNSIGNED_BYTE, pixel);

  printf("Pixel inside concave fill = (%d, %d, %d)\n",
         pixel[0], pixel[1], pixel[2]);
  if (pixel[1] > pixel[0] && pixel[1] > pixel[2]){
    printf("PASS: fill is green, as expected.\n");
  } else {
    printf("FAIL: fill is missing or wrong (expected green).\n");
  }

  pclose_ws(0);
  pclose_phigs();
  return 0;
}
