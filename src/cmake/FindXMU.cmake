FIND_LIBRARY(XMU_LIBRARY
             NAMES Xmu
             PATHS /usr/lib
             PATHS /usr/X11/lib
             PATHS /usr/openwin/lib
             PATHS /opt/homebrew/lib)

IF (XMU_LIBRARY)
    SET (XMU_FOUND TRUE)
ENDIF (XMU_LIBRARY)

IF (XMU_FOUND)
    MESSAGE (STATUS "Found Xmu:" ${XMU_LIBRARY})
ELSE (XMU_FOUND)
    MESSAGE (FATAL ERROR "Cannot find Xmu")
ENDIF (XMU_FOUND)

