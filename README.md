# OpenPHIGS

This repository is based on OpenPHIGS from

https://sourceforge.net/projects/phigs/files/

which is based on

https://www.x.org/archive/unsupported/lib/PHIGS

This distribution contains modifications and extensions which are needed for the purpose of data preservation in high energy physics.
The code maturity level is pre-alpha, since it does not implement the full standard, neither for the C bindings, nor for Fortran.

## Building

Run
```
make
make install
```
which will create and install the OpenPHIGS library, headers and tests in a new subdirectory called distrib. The install step will create a folder called distrib, including headers, binaries and libraries.

### Switching between GLEW and Epoxy
Openphigs uses by default GLEW. Switching between GLEW and Epoxy is supported by configuring with -DUSE_GLEW=0. The Makefile checks for the environment variable NO_GLEW. If defined, it will use Epoxy, else GLEW. To switch to Epoxy, do
```
NO_GLEW=1 make
```

### Switching between Xaw and Motif Widgets
By default, widgets will use Motif. To switch to Xaw, make sure that cmake is configure without the -DMotif switch. Take a look at the top level CMakeLists.txt file

## Extensions
Some extensions have been implemented, specifically for the Fortran bindings, to support the DELPHI and OPAL event displays.

### New Work station types 4 and 5
Work station types 4 (without double buffering) and 5 (with double buffering) take a screen shot when closed and write the screen shot as targa file to a file name.

#### Fortran bindings:
The second argument of popwk expects a non-zero integer which will be passed on as LUN.

* If a file name has been defined in the configuration file as  %wf <filename>, the targa file will be created with this name in the current folder.
* If no file name has been defined in the configuration file as %wf <filename> fort.<LUN> will be used.
* After opening the work station, the output file name can be set with a call to CALL PSFNAME(WKD, <filename>) where filename can be the full path to the output

#### C bindings

* If a file name has been defined in the configuration file as  %wf <filename>, the targa file will be created with this name in the current folder.
* If no file name has been defined in the configuraiton file, it defaults to "output.tga" and is created in the current directory.

## Transparency
The ALPHA channel in a structure can be set with

CALL PSALCH(X)

where X is a floating point number between 0. (fully transparent) and 1. (fully opaque).

## Migration notes and extensions
Some notes on differences with respect to other implementations.

### Workstation numbers
Workstation numbers can be in the range from 0 to 99.

### Workstation types
* 0 PWST_OUTPUT_TRUE                 Output only on GL display
* 1 PWST_OUTIN_TRUE                  Input/Output on GL display
* 2 PWST_OUTPUT_TRUE_DB              Output only on GL, buffered
* 3 PWST_OUTIN_TRUE_DB               Input/Output on GL display, buffered
* 4 PWST_HCOPY_TRUE_TGA              Hardcopy to file as TGA
* 5 PWST_HCOPY_TRUE_RGB_PNG          Hardcopy to file as PNG RGB only
* 6 PWST_HCOPY_TRUE_RGBA_PNG         Hardcopy to file as PNG with Alpha channel
* 7 PWST_HCOPY_TRUE_EPS              Hardcopy to file as Encapsulated PostScript, no shaders
* 8 PWST_HCOPY_TRUE_PDF              Hardcopy to file as PDF, no shaders
* 9 PWST_HCOPY_TRUE_SVG              Hardcopy to file as SVG, no shaders

Notes:
 * There is no support for PostScript at the moment.
 * The hardcopy types are available in Fortran. C-Bindings have not been tested with them

### Input devices
Openphigs behaviour on echo modes is summarized below.

#### String echo modes
* echo mode 1
  * opens a new window
  * echo area given in NC coordinates for the root window
* echo mode -1
  * echo area given in % relative to the main window

#### Valuator echo modes
* echo mode 1:
  * uses default strings for label, format low label and high label
  * opens a new window
  * echo area given in NC coordinates for the root window
* echo mode -1: As mode 1 but expects additional parameters as string, namely
  * label
  * format
  * low label
  * high label
  These should be encoded

Extensions:
* echo mode 2:
  * as echo mode 1 but places the window on top of the main window
  * echo area given as a fraction of the main window
* echo mode -2:
  * as echo mode -1 but places the window on top of the main window
  * echo area given as a fraction of the main window
* echo mode 3, -3:
  * as echo mode 2 but places the window on top of the main window
  * echo area given as a fraction of the main window
  * boxes the valuators up in one Window.
  (!) PPREC expects one integer for this echo mode which is the number of valuators to be boxed up

#### Choice echo modes
* echo mode 1:
  * opens a new window
  * echo area given in NC coordinates for the root window
* echo mode -1:
  * displays window on top of the root window
  * echo area is given as a fraction of the root window
* echo mode 3:
  * opens a new window
  * echo area given in NC coordinates for the root window
  * expects titels as strings
* echo mode -3:
  * as 3 but
  * echo area is given as a fraction of the root window
* echo mode 4:
  * as 3 but expects one more string
  * last string will be used as title
* echo mode -4:
  * as 4 but
  * echo area is given as a fraction of the root window

## Extensions
### There is some support for transparency.
#### Fortran bindings
* PXNDEF(STRING NAME): set the configuration location and file name
* PXSHCSF(INTEGER IWK, REAL VALUE): Set hardcopy scale factor for workstation ID WKID. Must be set before the workstation is being opened.
* PXQHCSF(INTEGER IWK, REAL VALUE): Inquire the current scale factor for workstation ID WKID. The value is returned in the second argument.

* PSALCH(REAL VALUE): set ALPHA channel to Value. Value is between 0(fully transparent) and 1 (opaque). Added to the current structure.
* PSFNAME(INTEGER IWK, CHARACTER FNAME): set output file name for workstation ID IWK

#### C-bindings
* pxset_conf_file_name(char* path): set the configuration location and file name
* pxset_conf_hcsf(WKID, Pfloat value): Set hardcopy scale factor for workstation ID WKID. Must be set before the workstation is being opened
* Pfloat pxinq_conf_hcsf(WKID): Inquire the current hardcopy scale factor for workstation ID WKID.

* pset_alpha_channel(float value): C-Binding for PSALCH. Added to the current structure.

## Versions
* 0.0.1-1: Revised code from upstream
* 0.0.2-1: Implement additional extend C and Fortran bindings for CERN specific purposes
* v0.1-1:  Fixes for exotic hardware
* v0.2-1:  Introduce scale factor for hardcopies
* v0.2-2:  Introduce additional functions to manage the scale factor programmatically
