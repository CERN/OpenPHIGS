/*
 * Checks what happens when MORE than MAX_FRAGMENTS (16, see
 * fs420_resolve.frag/fs430_resolve.frag) opaque fragments, all at exactly
 * the same depth (Z=0), cover the same pixel -- motivated by the DELPHI
 * dolphin logo (delgrasubs.f's KYDELP) stacking many overlapping colour
 * regions at Z=0 and, after the sortFragments() depth-tie fix, showing only
 * outlines with no fill.
 *
 * Draws N (default 20, > MAX_FRAGMENTS) fully overlapping opaque quads at
 * Z=0, each a distinct shade of grey increasing with draw order, and reports
 * the centre pixel. It should read as shade N-1 (the last one drawn, i.e.
 * nearly white) if "last drawn wins" continues to hold once the per-pixel
 * fragment count exceeds MAX_FRAGMENTS.
 *
 * Build/run: see tie_break_test.c in this directory for the exact command.
 * Optional argument: N (number of overlapping quads), default 20.
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

int main(int argc, char ** argv)
{
  int n = (argc > 1) ? atoi(argv[1]) : 20;
  Ppoint pts[4] = { {0.2, 0.2}, {0.8, 0.2}, {0.8, 0.8}, {0.2, 0.8} };
  Ppoint_list list = { 4, pts };
  Pgcolr colr;
  GLubyte pixel[3];
  int width, height, i;
  int expected;

  colr.type = PMODEL_RGB;

  popen_phigs(NULL, 0);
  popen_struct(1);
  pset_int_style(PSTYLE_SOLID);
  for (i = 0; i < n; i++){
    float shade = (float) (i + 1) / (float) n; /* 1/n .. 1.0, increasing */
    colr.val.general.x = shade;
    colr.val.general.y = shade;
    colr.val.general.z = shade;
    pset_int_colr(&colr);
    pset_back_int_colr(&colr);
    pfill_area(&list); /* same quad, same Z=0, drawn again on top */
  }
  pclose_struct();

  popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
  ppost_struct(0, 1, 0.0);
  pupd_ws(0, PFLAG_PERFORM);

  width = PHG_WSID(0)->ws_rect.width;
  height = PHG_WSID(0)->ws_rect.height;
  glReadBuffer(GL_FRONT); /* pupd_ws() already swapped */
  glReadPixels(width / 2, height / 2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

  expected = 255; /* last draw, shade = n/n = 1.0 -> white */
  printf("N=%d overlapping opaque quads at Z=0. Centre pixel = (%d, %d, %d)\n",
         n, pixel[0], pixel[1], pixel[2]);
  printf("Expected (last draw, shade 1.0): (%d, %d, %d)\n",
         expected, expected, expected);
  if (pixel[0] >= expected - 2 && pixel[1] >= expected - 2 && pixel[2] >= expected - 2){
    printf("PASS: last-drawn (brightest) shade is on top.\n");
  } else {
    printf("FAIL: some earlier-drawn (darker) shade is on top instead,"
           " or fill is missing.\n");
  }

  pclose_ws(0);
  pclose_phigs();
  return 0;
}
