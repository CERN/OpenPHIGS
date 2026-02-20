CMAKE_MINIMUM_REQUIRED(VERSION 3.9...3.31)
PROJECT(PHIGS)

SET(CMAKE_MODULE_PATH ${PHIGS_SOURCE_DIR}/cmake)

FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(X11 REQUIRED)
FIND_PACKAGE(XMU REQUIRED)
FIND_PACKAGE(Motif REQUIRED)
FIND_PACKAGE(GLEW)
FIND_PACKAGE(epoxy)

if (APPLE)
  set(CMAKE_C_COMPILER gcc)
  set(CMAKE_C_COMPILER_ID GNU)
  set(CMAKE_CPP_COMPILER cpp)
  set(CMAKE_Fortran_COMPILER gfortran)
  set(CMAKE_EXE_LINKER_FLAGS  "-Wl,-ld_classic,-dead_strip")
endif()

set(OPENGL_GL_PREFERENCE "GLVND")

# Choose between Epoxy or GLEW
if (USE_GLEW)
set(USE_GLEW ON)
else()
set(USE_GLEW OFF)
endif()
if (OPENPHIGS_FORCE_32)
  set(CMAKE_VERBOSE_MAKEFILE ON)
  list(PREPEND CMAKE_SYSTEM_LIBRARY_PATH "/usr/lib")
  message(STATUS "${CMAKE_SYSTEM_LIBRARY_PATH}")
  set( FIND_LIBRARY_USE_LIB32_PATHS  ON)
  set( FIND_LIBRARY_USE_LIB64_PATHS  OFF)
  set(CMAKE_SIZEOF_VOID_P 4)
  STRING(REPLACE "lib64" "lib" OPENSSL_CRYPTO_LIBRARY "${OPENSSL_CRYPTO_LIBRARY}")
  message(STATUS "PHIGS: Guess for 32bit: OPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}")
endif()

set(GLEW_VERBOSE ON)

# for debugging
add_definitions(-g)
# add_definitions(-DTEST)
# add_definitions(-DDEBUG)
# add_definitions(-DDEBUGINP)

#
# use Motif widgets rather than Xaw
add_definitions(-DMOTIF)

# use GLEW or epoxy
if (USE_GLEW)
  add_definitions(-DGLEW)
  set (GL_INCLUDES ${GLEW_INCLUDE_DIR})
else()
  add_definitions(-DEpoxy)
  set (GL_INCLUDES ${Epoxy_INCLUDE_DIR})
endif()

# gl2ps
FIND_PATH(GL2PS_INCLUDE_DIR gl2ps.h)
FIND_LIBRARY(GL2PS_LIBRARY NAMES gl2ps)
add_definitions(-DGLEW)
set (GL_INCLUDES ${GL2PS_INCLUDE_DIR})

message(STATUS "GL Includes=${GL_INCLUDES}")
include_directories(${X11_INCLUDE_DIR} ${MOTIF_INCLUDE_DIR} ${GL_INCLUDES})

SET(CMAKE_C_FLAGS  "$ENV{CFLAGS} -DMOTIF -g -O0 -fPIC")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -DMOTIF -g -O0  -fPIC")
SET(CMAKE_Fortran_FLAGS "$ENV{FFLAGS} -DMOTIF -finit-local-zero -g -O0 -fPIC")

SET(CMAKE_EXE_LINKER_FLAGS $ENV{LDFLAGS})
if (OPENPHIGS_FORCE_32)
  set(CMAKE_C_FLAGS " -m32 ${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS " -m32 ${CMAKE_CXX_FLAGS}")
  set(CMAKE_Fortran_FLAGS " -m32 ${CMAKE_Fortran_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS  "-m32 ${CMAKE_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

if (APPLE)
INCLUDE_DIRECTORIES(
    /opt/X11/include
    ${PHIGS_SOURCE_DIR}/include/phigs
    )
    set (XLIBS "-L/opt/X11/lib -lX11 -lGL")
else()
INCLUDE_DIRECTORIES(
    ${PHIGS_SOURCE_DIR}/include/phigs
    )
    set (XLIBS "")
endif()

if (USE_GLEW)
  SET(PHIGS_LIBRARIES
      phigs
      ${XLIBS}
      ${MOTIF_LIBRARIES}
      ${XMU_LIBRARY}
      ${PNG_LIBRARY}
      ${X11_Xaw_LIB}
      ${X11_Xt_LIB}
      ${X11_LIBRARIES}
      ${GLEW_LIBRARIES}
      ${OPENGL_LIBRARIES}
      ${GL2PS_LIBRARY}
      m
      )
else()
  SET(PHIGS_LIBRARIES
      phigs
      ${XLIBS}
      ${MOTIF_LIBRARIES}
      ${XMU_LIBRARY}
      ${PNG_LIBRARY}
      ${X11_Xaw_LIB}
      ${X11_Xt_LIB}
      ${X11_LIBRARIES}
      ${OPENGL_LIBRARIES}
      ${Epoxy_LIBRARY}
      ${GL2PS_LIBRARY}
      m
      )
endif()
message(STATUS "PHIGS: Linking with libraries ${PHIGS_LIBRARIES}")
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(libphigs)
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(test_c)
ADD_SUBDIRECTORY(test_f)

# uncomment for debugging
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
