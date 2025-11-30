cmake_minimum_required(VERSION 3.30.0)
project(fxng)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory(deps/glfw)

set(glew-cmake_BUILD_SHARED OFF)
set(glew-cmkae_BUILD_STATIC ON)
set(USE_GLU OFF)
set(GLEW_OSMESA OFF)
set(PKG_CONFIG_REPRESENTATIVE_TARGET OFF)
set(ONLY_LIBS ON)

add_subdirectory(deps/glew)

add_subdirectory(engine)
add_subdirectory(game)
