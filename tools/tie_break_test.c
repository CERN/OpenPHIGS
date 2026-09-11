/*
 * Direct validation for the OIR depth-tie compositing bug: two opaque fill
 * areas at the same Z (PHIGS 2D primitives, like a banner box and the text
 * drawn on top of it in delgrasubs.f, are implicitly Z=0), overlapping,
 * drawn one after the other. With order independent rendering on, the
 * fragment drawn LAST must still end up on top when depths tie -- otherwise
 * whatever was drawn first (e.g. a banner's background box) hides whatever
 * was drawn after it (e.g. the banner's text), which is exactly the bug
 * reported against src/libphigs/shaders/fs420_resolve.frag and
 * fs430_resolve.frag's sortFragments().
 *
 * Draws a RED quad first, then an overlapping BLUE quad second, and prints
 * the colour at the centre of the window, which lies in the overlap region.
 * PASS: centre reads blue (the fix: later draws win ties).
 * FAIL: centre reads red (the bug: first draw wins ties).
 *
 * Build against the project's own build tree, e.g. from build/:
 *   gcc -I../src/include/phigs -I include/phigs \
 *       ../tools/tie_break_test.c -o tie_break_test \
 *       libphigs/libphigs.a -lXm -lXmu -lXaw -lXt -lSM -lICE -lX11 -lXext \
 *       -lGLEW -lOpenGL -lGLX -lGLU -lgl2ps -lm
 * Run from the directory containing phigs.def/phigs.conf:
 *   ./tie_break_test
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
  Ppoint red_pts[4]  = { {0.2, 0.2}, {0.7, 0.2}, {0.7, 0.7}, {0.2, 0.7} };
  Ppoint blue_pts[4] = { {0.3, 0.3}, {0.8, 0.3}, {0.8, 0.8}, {0.3, 0.8} };
  Ppoint_list red_list  = { 4, red_pts };
  Ppoint_list blue_list = { 4, blue_pts };
  Pgcolr red, blue;
  GLubyte pixel[3];
  int width, height;

  red.type = PMODEL_RGB;
  red.val.general.x = 1.0; red.val.general.y = 0.0; red.val.general.z = 0.0;
  blue.type = PMODEL_RGB;
  blue.val.general.x = 0.0; blue.val.general.y = 0.0; blue.val.general.z = 1.0;

  popen_phigs(NULL, 0);
  popen_struct(1);
  pset_int_style(PSTYLE_SOLID);
  pset_int_colr(&red);
  pset_back_int_colr(&red);
  pfill_area(&red_list);
  pset_int_colr(&blue);
  pset_back_int_colr(&blue);
  pfill_area(&blue_list); /* drawn after red, overlaps it, same Z=0 */
  pclose_struct();

  popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
  ppost_struct(0, 1, 0.0);
  pupd_ws(0, PFLAG_PERFORM);

  width = PHG_WSID(0)->ws_rect.width;
  height = PHG_WSID(0)->ws_rect.height;
  /* pupd_ws() already swapped buffers, so the rendered frame is now in
     front, not back -- reading GL_BACK here would see invalidated data */
  glReadBuffer(GL_FRONT);
  /* (0.5, 0.5) in NDF lands in the overlap of both quads */
  glReadPixels(width / 2, height / 2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

  printf("Centre pixel (overlap region) = (%d, %d, %d)\n",
         pixel[0], pixel[1], pixel[2]);
  if (pixel[2] > pixel[0]){
    printf("PASS: blue (drawn second) is on top, as it should be.\n");
  } else if (pixel[0] > pixel[2]){
    printf("FAIL: red (drawn first) is on top -- the tie-break bug.\n");
  } else {
    printf("INCONCLUSIVE: neither colour clearly wins (%d,%d,%d).\n",
           pixel[0], pixel[1], pixel[2]);
  }

  pclose_ws(0);
  pclose_phigs();
  return 0;
}
