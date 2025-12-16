file(GLOB_RECURSE SRC src/*.c src/*.cxx)

add_library(fxng STATIC ${SRC})
target_include_directories(fxng PUBLIC include)
target_link_libraries(fxng PUBLIC common yaml-cpp::yaml-cpp glal)

if (${FXNG_PACKAGE})
    target_compile_definitions(fxng PUBLIC FXNG_PACKAGE)
endif ()
