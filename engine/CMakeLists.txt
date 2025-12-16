file(GLOB_RECURSE SRC src/*.c src/*.cxx)

add_library(fxng STATIC ${SRC})
target_include_directories(fxng PUBLIC include)
target_link_libraries(fxng PUBLIC common glal glm yaml-cpp)

if (${FXNG_PACKAGE})
    target_compile_definitions(fxng PUBLIC FXNG_PACKAGE)
endif ()
