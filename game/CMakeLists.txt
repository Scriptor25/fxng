file(GLOB_RECURSE SRC src/*.c src/*.cxx)

add_executable(game ${SRC})
target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE fxng)
