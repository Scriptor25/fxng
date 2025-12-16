file(GLOB_RECURSE SRC src/*.c src/*.cxx)

add_library(glal STATIC ${SRC})
target_include_directories(glal PUBLIC include)
target_link_libraries(glal PUBLIC common glfw GLEW::GLEW OpenGL::GL Vulkan::Vulkan)
