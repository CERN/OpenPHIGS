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

Run (`./oir_repro --help`-style usage is printed at the top of the source):

    ./oir_repro                        # baseline: matches OpenPHIGS's own setup
    ./oir_repro cleartex finish unit6  # combine variants to rule out theories

Exit status: `0` = PASS (shader agrees with the CPU-side ground truth), `1` =
FAIL (reproduces the bug), `2` = setup error. Useful for comparing vendors/
drivers, and as the basis for a driver bug report.
