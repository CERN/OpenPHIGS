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
change. Not yet ported into `wsgl_oir.c`/the fs430 shaders.

Exit status: `0` = PASS, `1` = FAIL (reproduces the bug), `2` = setup error.
OIR renders correctly on Mesa/Intel; useful for comparing vendors/drivers.
