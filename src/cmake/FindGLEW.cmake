FIND_LIBRARY(GLEW_LIBRARIES
             NAMES GLEW
             PATHS /usr/lib
             PATHS /usr/X11/lib
             PATHS /usr/openwin/lib
             PATHS /opt/homebrew/lib)

IF (GLEW_LIBRARIES)
    SET (GLEW_FOUND TRUE)
ENDIF (GLEW_LIBRARIES)

IF (GLEW_FOUND)
    MESSAGE (STATUS "Found GLEW:" ${GLEW_LIBRARIES})
ELSE (GLEW_FOUND)
    MESSAGE (FATAL ERROR "Cannot find GLEW")
ENDIF (GLEW_FOUND)
