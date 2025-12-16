cmake_minimum_required(VERSION 3.30.0)
project(fxng)

find_package(yaml-cpp REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(glal)
add_subdirectory(engine)
add_subdirectory(game)
