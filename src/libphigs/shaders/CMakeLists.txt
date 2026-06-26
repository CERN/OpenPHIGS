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
add_custom_target(generate_shaders_vs120 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/vs120.h)
add_custom_target(generate_shaders_vs130 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/vs130.h)
add_custom_target(generate_shaders_fs120 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/fs120.h)
add_custom_target(generate_shaders_fs130 DEPENDS ${CMAKE_BINARY_DIR}/include/phigs/private/fs130.h)
add_dependencies(phigs generate_shaders_vs120 generate_shaders_vs130 generate_shaders_fs120 generate_shaders_fs130)
add_dependencies(phigs-shared generate_shaders_vs120 generate_shaders_vs130 generate_shaders_fs120 generate_shaders_fs130)
