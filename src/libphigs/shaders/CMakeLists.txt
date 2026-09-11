add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/vs120.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs120.vert vertex_shader_text_120 > ${CMAKE_BINARY_DIR}/include/phigs/private/vs120.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs120.vert
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/fs120.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs120.frag fragment_shader_text_120 > ${CMAKE_BINARY_DIR}/include/phigs/private/fs120.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs120.frag
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/vs130.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs130.vert vertex_shader_text_130 > ${CMAKE_BINARY_DIR}/include/phigs/private/vs130.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs130.vert
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/fs130.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs130.frag fragment_shader_text_130 > ${CMAKE_BINARY_DIR}/include/phigs/private/fs130.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs130.frag
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/vs420.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs420.vert vertex_shader_text_420 > ${CMAKE_BINARY_DIR}/include/phigs/private/vs420.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs420.vert
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/fs420.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs420.frag fragment_shader_text_420 > ${CMAKE_BINARY_DIR}/include/phigs/private/fs420.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs420.frag
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/vs420_resolve.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs420_resolve.vert vertex_shader_text_420_resolve > ${CMAKE_BINARY_DIR}/include/phigs/private/vs420_resolve.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs420_resolve.vert
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/fs420_resolve.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs420_resolve.frag fragment_shader_text_420_resolve > ${CMAKE_BINARY_DIR}/include/phigs/private/fs420_resolve.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs420_resolve.frag
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/vs430.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs430.vert vertex_shader_text_430 > ${CMAKE_BINARY_DIR}/include/phigs/private/vs430.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs430.vert
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/fs430.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs430.frag fragment_shader_text_430 > ${CMAKE_BINARY_DIR}/include/phigs/private/fs430.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs430.frag
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/vs430_resolve.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs430_resolve.vert vertex_shader_text_430_resolve > ${CMAKE_BINARY_DIR}/include/phigs/private/vs430_resolve.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/vs430_resolve.vert
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/include/phigs/private/fs430_resolve.h
    COMMAND ${CMAKE_SOURCE_DIR}/tools/create_shader_include_files.sh
            ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs430_resolve.frag fragment_shader_text_430_resolve > ${CMAKE_BINARY_DIR}/include/phigs/private/fs430_resolve.h
    DEPENDS ${CMAKE_SOURCE_DIR}/libphigs/shaders/fs430_resolve.frag
)
add_custom_target(generate_shaders_vs420r DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/vs420_resolve.h)
add_custom_target(generate_shaders_fs420r DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/fs420_resolve.h)
add_custom_target(generate_shaders_vs430r DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/vs430_resolve.h)
add_custom_target(generate_shaders_fs430r DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/fs430_resolve.h)

add_dependencies(phigs        generate_shaders_vs420r generate_shaders_fs420r)
add_dependencies(phigs-shared generate_shaders_vs420r generate_shaders_fs420r)
add_dependencies(phigs-obj    generate_shaders_vs420r generate_shaders_fs420r)

add_dependencies(phigs        generate_shaders_vs430r generate_shaders_fs430r)
add_dependencies(phigs-shared generate_shaders_vs430r generate_shaders_fs430r)
add_dependencies(phigs-obj    generate_shaders_vs430r generate_shaders_fs430r)

add_custom_target(generate_shaders_vs120 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/vs120.h)
add_custom_target(generate_shaders_vs130 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/vs130.h)
add_custom_target(generate_shaders_vs420 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/vs420.h)
add_custom_target(generate_shaders_vs430 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/vs430.h)

add_custom_target(generate_shaders_fs120 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/fs120.h)
add_custom_target(generate_shaders_fs130 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/fs130.h)
add_custom_target(generate_shaders_fs420 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/fs420.h)
add_custom_target(generate_shaders_fs430 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/fs430.h)

add_dependencies(phigs
  generate_shaders_vs120 generate_shaders_vs130 generate_shaders_vs420 generate_shaders_vs430
  generate_shaders_fs120 generate_shaders_fs130 generate_shaders_fs420 generate_shaders_fs430
)
add_dependencies(phigs-shared
  generate_shaders_vs120 generate_shaders_vs130 generate_shaders_vs420 generate_shaders_vs430
  generate_shaders_fs120 generate_shaders_fs130 generate_shaders_fs420 generate_shaders_fs430
)
add_dependencies(phigs-obj
  generate_shaders_vs120 generate_shaders_vs130 generate_shaders_vs420 generate_shaders_vs430
  generate_shaders_fs120 generate_shaders_fs130 generate_shaders_fs420 generate_shaders_fs430
)
