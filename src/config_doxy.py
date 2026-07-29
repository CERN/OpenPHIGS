import re

path = 'c:/Users/HP/Desktop/GSOC_Prop/cern_contri/OpenPHIGS/src/Doxyfile.in'
with open(path, 'r') as f:
    text = f.read()

text = re.sub(r'^PROJECT_NAME\s*=.*', 'PROJECT_NAME = "OpenPHIGS"', text, flags=re.MULTILINE)
text = re.sub(r'^OUTPUT_DIRECTORY\s*=.*', 'OUTPUT_DIRECTORY = @CMAKE_CURRENT_BINARY_DIR@/doc', text, flags=re.MULTILINE)
text = re.sub(r'^INPUT\s*=.*', 'INPUT = @CMAKE_CURRENT_SOURCE_DIR@/include/phigs/phigs.h @CMAKE_CURRENT_SOURCE_DIR@/include/phigs/phigsf77.h @CMAKE_CURRENT_SOURCE_DIR@/include/phigs/phgtype.h @CMAKE_CURRENT_SOURCE_DIR@/include/phigs/errnum.h @CMAKE_CURRENT_SOURCE_DIR@/libphigs/c_binding @CMAKE_CURRENT_SOURCE_DIR@/libphigs/f_binding', text, flags=re.MULTILINE)
text = re.sub(r'^GENERATE_MAN\s*=.*', 'GENERATE_MAN = YES', text, flags=re.MULTILINE)
text = re.sub(r'^GENERATE_HTML\s*=.*', 'GENERATE_HTML = YES', text, flags=re.MULTILINE)
text = re.sub(r'^OPTIMIZE_OUTPUT_FOR_C\s*=.*', 'OPTIMIZE_OUTPUT_FOR_C = YES', text, flags=re.MULTILINE)
text = re.sub(r'^EXTRACT_ALL\s*=.*', 'EXTRACT_ALL = NO', text, flags=re.MULTILINE)
text = re.sub(r'^RECURSIVE\s*=.*', 'RECURSIVE = YES', text, flags=re.MULTILINE)

with open(path, 'w') as f:
    f.write(text)
