cmake_minimum_required(VERSION 2.8)
project(DriverBugs C CXX)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# GLFW, only build the library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${SRC_DIR}/external/glfw)

add_library(glad STATIC
    ${SRC_DIR}/external/glad/src/glad.c
    ${SRC_DIR}/external/glad/include/KHR/khrplatform.h
    ${SRC_DIR}/external/glad/include/glad/glad.h
)
target_include_directories(glad PUBLIC ${SRC_DIR}/external/glad/include)

add_library(utils STATIC
    ${SRC_DIR}/utils/GLFWApp.cpp
    ${SRC_DIR}/utils/GLFWApp.h
    ${SRC_DIR}/utils/GLFW.h
    ${SRC_DIR}/utils/Shader.cpp
    ${SRC_DIR}/utils/Shader.h
)
target_link_libraries(utils glfw glad)
target_include_directories(utils PUBLIC ${SRC_DIR})

set_target_properties(utils PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON    
)

add_executable(uniform_order ${SRC_DIR}/uniform_order/Main.cpp)
target_link_libraries(uniform_order utils)
set_target_properties(uniform_order PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

add_executable(gl_secondary_frag_data ${SRC_DIR}/gl_secondary_frag_data/Main.cpp)
target_link_libraries(gl_secondary_frag_data utils)
set_target_properties(gl_secondary_frag_data PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

add_executable(uniform_for_local_variable ${SRC_DIR}/uniform_for_local_variable/Main.cpp)
target_link_libraries(uniform_for_local_variable utils)
set_target_properties(uniform_for_local_variable PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

add_executable(blend_func_extended_array ${SRC_DIR}/blend_func_extended_array/Main.cpp)
target_link_libraries(blend_func_extended_array utils)
set_target_properties(blend_func_extended_array PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

add_executable(fbo_completness_caching ${SRC_DIR}/fbo_completness_caching/Main.cpp)
target_link_libraries(fbo_completness_caching utils)
set_target_properties(fbo_completness_caching PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

add_executable(frag_depth_clamp_32f_depth ${SRC_DIR}/frag_depth_clamp_32f_depth/Main.cpp)
target_link_libraries(frag_depth_clamp_32f_depth utils)
set_target_properties(frag_depth_clamp_32f_depth PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

add_executable(texture_cube_specification_order ${SRC_DIR}/texture_cube_specification_order/Main.cpp)
target_link_libraries(texture_cube_specification_order utils)
set_target_properties(texture_cube_specification_order PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

add_executable(map_buffer_range_gpu_hang ${SRC_DIR}/map_buffer_range_gpu_hang/Main.cpp)
target_link_libraries(map_buffer_range_gpu_hang utils)
set_target_properties(map_buffer_range_gpu_hang PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

add_executable(blitframebuffer_overflow ${SRC_DIR}/blitframebuffer_overflow/Main.cpp)
target_link_libraries(blitframebuffer_overflow utils)
set_target_properties(blitframebuffer_overflow PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    )
target_compile_definitions(blitframebuffer_overflow PRIVATE NOMINMAX)
