SET(P_ARCHIVE_SRCS
  archive/ar.c
  archive/ar_conv.c
  archive/ar_ops.c
  archive/ar_hier.c
)

SET(P_C_BINDING_SRCS
  c_binding/cb.c
  c_binding/cb_ar.c
  c_binding/cb_el.c
  c_binding/cb_lite.c
  c_binding/cb_phg.c
  c_binding/cb_text.c
  c_binding/cb_ws.c
  c_binding/cb_conf.c
  c_binding/cb_extel.c
  c_binding/cb_inp.c
  c_binding/cb_pgse.c
  c_binding/cb_str.c
  c_binding/cb_tr.c
  c_binding/cb_wst.c
)

SET(P_F_BINDING_SRCS
  f_binding/fb_ar.c
  f_binding/fb_conf.c
  f_binding/fb_el.c
  f_binding/fb_extel.c
  f_binding/fb_inp.c
  f_binding/fb_pgse.c
  f_binding/fb_phg.c
  f_binding/fb_str.c
  f_binding/fb_text.c
  f_binding/fb_tr.c
  f_binding/fb_ws.c
  f_binding/fb_wst.c
)

SET(P_CSS_SRCS
  css/css_el.c
  css/css_ini.c
  css/css_inq.c
  css/css_pr.c
  css/css_set.c
  css/css_stb.c
  css/css_str.c
  css/css_ws.c
)

SET(P_UT_SRCS
  ut/ut.c
  ut/ut_conv.c
  ut/ut_htab.c
  ut/ut_list.c
  ut/ut_mat.c
  ut/ut_nset.c
  ut/ut_stk.c
)

SET(P_PHG_SRCS
  phg/phg_attr.c
  phg/phg.c
  phg/phg_swap.c
)

SET(P_WSGL_SRCS
  wsgl/wsgl_attr.c
  wsgl/wsgl.c
  wsgl/wsgl_clear.c
  wsgl/wsgl_edge.c
  wsgl/wsgl_extattr.c
  wsgl/wsgl_fasd3clear.c
  wsgl/wsgl_fasd3edge.c
  wsgl/wsgl_fasd3fill.c
  wsgl/wsgl_fasdclear.c
  wsgl/wsgl_fasdfill.c
  wsgl/wsgl_fasedge.c
  wsgl/wsgl_fill.c
  wsgl/wsgl_hatch.c
  wsgl/wsgl_light.c
  wsgl/wsgl_line.c
  wsgl/wsgl_marker.c
  wsgl/wsgl_obj.c
  wsgl/wsgl_shaders.c
  wsgl/wsgl_sofas3clear.c
  wsgl/wsgl_sofas3edge.c
  wsgl/wsgl_sofas3fill.c
  wsgl/wsgl_text.c
)

SET(P_SIN_SRCS
  input/sin.c
  input/sin_cho.c
  input/sin_cvs.c
  input/sin_dev.c
  input/sin_evt.c
  input/sinq.c
  input/sin_str.c
  input/sin_val.c
  input/sin_ws.c
)

SET(P_WS_SRCS
  ws/wsb.c
  ws/wsb_lut.c
  ws/ws_inp.c
  ws/ws_pm.c
  ws/wst.c
  ws/wstx_ini.c
  ws/wsx.c
  ws/wsx_inp.c
  ws/wsx_util.c
)

SET(P_FONTS_SRCS
  fonts/fnt.c
  fonts/fnt_mroman.c
  fonts/fnt_roman.c
)

SET(P_ERRORS_SRCS
  errors/err.c
  errors/err_num.c
)

SET(P_CONF_SRCS
  conf/ophconf.c
)

SET(P_UTILS_SRCS
  utils/hdl.c
  utils/psl.c
  utils/fasd3.c
  utils/sofas3.c
)

SET(P_CP_SRCS
  cp/cpm_tlkt.c
)

SET(OPHIGS_ALL
  ${P_ARCHIVE_SRCS}
  ${P_C_BINDING_SRCS}
  ${P_F_BINDING_SRCS}
  ${P_CSS_SRCS}
  ${P_UT_SRCS}
  ${P_PHG_SRCS}
  ${P_WSGL_SRCS}
  ${P_SIN_SRCS}
  ${P_WS_SRCS}
  ${P_ERRORS_SRCS}
  ${P_FONTS_SRCS}
  ${P_CONF_SRCS}
  ${P_UTILS_SRCS}
  ${P_CP_SRCS}
)

add_library(phigs STATIC ${OPHIGS_ALL})
add_library(phigs-shared SHARED ${OPHIGS_ALL})
ADD_SUBDIRECTORY(shaders)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include/phigs)
if (APPLE)
  set_target_properties(phigs-shared PROPERTIES OUTPUT_NAME phigs LINK_FLAGS "-undefined dynamic_lookup -flat_namespace")
else()
  set_target_properties(phigs-shared PROPERTIES OUTPUT_NAME phigs LINK_FLAGS "--shared")
  target_link_libraries(phigs-shared ${X11_LIBRARIES})
  target_link_libraries(phigs-shared ${XLIBS})
  target_link_libraries(phigs-shared ${X11_Xaw_LIB})
  target_link_libraries(phigs-shared ${X11_Xt_LIB})
  target_link_libraries(phigs-shared ${MOTIF_LIBRARIES})
  target_link_libraries(phigs-shared ${OPENGL_LIBRARIES})
  target_link_libraries(phigs-shared ${XMU_LIBRARY})
  target_link_libraries(phigs-shared ${PNG_LIBRARY})
if (USE_GLEW)
  target_link_libraries(phigs-shared ${GLEW_LIBRARIES})
else()
  target_link_libraries(phigs-shared ${EPOXY_LIBRARIES})
endif()
  target_link_libraries(phigs-shared ${GL2PS_LIBRARY})
  target_link_libraries(phigs-shared ${MATH_LIBRARY})
endif()

INSTALL(TARGETS
  phigs
  phigs-shared
  ARCHIVE DESTINATION lib
)
