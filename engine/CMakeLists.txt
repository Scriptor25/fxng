file(GLOB_RECURSE SRC src/*.c src/*.cxx)

find_package(glfw3 CONFIG REQUIRED)

add_library(fxng STATIC ${SRC})
target_include_directories(fxng PUBLIC include)
target_link_libraries(fxng PUBLIC glfw libglew_static glm yaml-cpp)

if (${FXNG_PACKAGE})
    target_compile_definitions(fxng PUBLIC FXNG_PACKAGE)
endif ()
