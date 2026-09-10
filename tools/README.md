# tools/

Standalone diagnostic programs, not part of the OpenPHIGS library or its
build. Build and run them individually as needed.

## oir_repro.c

Minimal, dependency-light repro for a suspected NVIDIA driver bug affecting
order independent rendering (`%oir` in `phigs.def`): a `GL_R32UI` image
cleared through the normal texture-update path and bound to an image unit is
not seen correctly by `imageLoad()` in a separately linked GLSL program, even
though a CPU-side `glGetTexImage()` readback confirms the clear did reach the
texture. Observed so far on an NVIDIA RTX 3060 (driver 580.178.04); OIR works
correctly on Mesa/Intel.

Build:

    gcc oir_repro.c -o oir_repro -lX11 -lGL -lepoxy

The default mode (`image`) matches OpenPHIGS's own setup and reproduces the
bug; flags `cleartex`/`finish`/`vbo`/`unit6` vary it without changing the
result. A handful of other modes each try a specific workaround or isolate a
different part of the pipeline:

    ./oir_repro                # baseline: matches wsgl_oir.c's own approach -> FAIL
    ./oir_repro uniform        # + explicit glUniform1i on the image unit    -> FAIL
    ./oir_repro core           # core-profile context instead of compat     -> FAIL
    ./oir_repro sso            # separable shader objects / pipeline        -> FAIL
    ./oir_repro ssbo           # head pointer as a std430 SSBO, not uimage2D -> PASS
    ./oir_repro listbuf        # the uimageBuffer approach list_buffer uses -> PASS

Tested on an NVIDIA RTX 3060 (driver 580.178.04): the bug is specific to a 2D
`uimage2D` bound as an image and read via `imageLoad()` in a separately
linked GLSL program; buffer-backed images (`uimageBuffer`, and plain SSBOs)
are unaffected on this driver. That means only `head_pointer_image` needs to
move from a 2D `R32UI` texture to an SSBO indexed by `y * width + x` --
`list_buffer` (already a `uimageBuffer`/`GL_TEXTURE_BUFFER`) does not need to
change. This fix has since been ported into `wsgl_oir.c` and the fs420/fs430
shaders (`ws->oir.head_p_buffer`).

Exit status: `0` = PASS, `1` = FAIL (reproduces the bug), `2` = setup error.
OIR renders correctly on Mesa/Intel; useful for comparing vendors/drivers.

## tie_break_test.c

Direct validation for a depth-tie compositing bug in
`fs420_resolve.frag`/`fs430_resolve.frag`'s `sortFragments()`: two opaque
fill areas at exactly the same Z (PHIGS 2D primitives, like a banner
background box and the text drawn over it, are implicitly Z=0), overlapping,
drawn one after the other. With order independent rendering on, whichever
was drawn *last* must still end up on top when depths tie -- `createFragmentList()`
walks its linked list newest-first, and a plain `<` comparison in the bubble
sort left exact ties in that order, so the *oldest* draw ended up composited
last (i.e. on top) instead. Symptom in practice: a banner's background box
hiding the text and logo drawn after it. Fixed by using `<=` instead of `<`,
which reverses tied elements too.

Build (from `build/`, against the project's own build tree):

    gcc -DGLEW -DMOTIF -I../src/include/phigs -Iinclude/phigs \
        ../tools/tie_break_test.c -o tie_break_test \
        libphigs/libphigs.a -lXm -lXmu -lpng -lXaw -lXt -lSM -lICE -lX11 -lXext \
        -lGLEW -lOpenGL -lGLX -lGLU -lgl2ps -lm

Run from the directory containing `phigs.def`/`phigs.conf`. Prints the
colour at the centre of the window (the overlap region): PASS if it reads
blue (the fragment drawn second), FAIL if it reads red (the one drawn
first) -- confirmed to flip between the two by toggling `<`/`<=` in
`sortFragments()`.

## concave_fill_test.c, overflow_tie_test.c, fasd_fill_test.c, fasd_stack_test.c

Follow-on checks built while chasing a report that, after the
`sortFragments()` fix above, the DELPHI dolphin logo (`delgrasubs.f`'s
`KYDELP`) lost its fill under OIR (outline only). Same build command as
`tie_break_test.c`.

- `concave_fill_test.c`: a single concave (non-convex) `pfill_area()` fill
  at Z=0 -- PASS, tessellated concave fills are not the problem on their own.
- `overflow_tie_test.c N` (default 20): N fully overlapping opaque quads at
  Z=0. PASS up to the point where N times the quad's pixel count exceeds the
  workstation's total OIR `list_capacity` (`%lpp` in `phigs.def` times the
  pixel count) -- past that it fails with an
  `OIR fragment list overflowed` warning, which is the existing, unrelated,
  documented overflow behaviour, not a new bug.
- `fasd_fill_test.c [disting] [nohlhsr]`: the same concave shape as
  `concave_fill_test.c`, but drawn with `pfill_area_set_data()` (PFASD in
  Fortran -- what `KYDELP` actually uses) instead of `pfill_area()`.
- `fasd_stack_test.c`: three overlapping opaque PFASD fills (green, blue,
  red) at Z=0, mirroring `tie_break_test.c` but through the PFASD path.

While investigating, found and fixed a real bug in `wsgl_fasdfill.c`:
`wsgl_fill_area_set_data_front()`/`_back()` unconditionally applied
`glPolygonOffset` (meant to stop a fill from Z-fighting with edges drawn
over it), even with `GL_DEPTH_TEST` disabled (`PHIGS_HLHSR_MODE_NONE`,
common for 2D/annotation-style content, including this banner). With depth
testing off, that offset serves no purpose but still perturbs
`gl_FragCoord.z`, which OIR always sorts by -- turning an intended exact
tie against the rest of a same-Z 2D scene into a genuine (small) depth
difference. Fixed by only applying the offset when depth testing is
actually on, matching `pfill_area()`'s plain 2D path, which never applied
it at all. `fasd_fill_test.c`/`fasd_stack_test.c` pass with and without this
fix in the isolated cases tried so far, so it is a real, defensible
correctness fix but not confirmed as the complete explanation for the
dolphin's specific symptom -- retest against the real application.
