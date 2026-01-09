cmake_minimum_required(VERSION 3.10)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/soil)
add_subdirectory(3rdparty/glm)

add_library(glad STATIC 3rdparty/glad/src/glad.c
        3rdparty/stb/stb_image.cpp)

add_library(imgui STATIC
        3rdparty/imgui/imgui.cpp
        3rdparty/imgui/imgui_demo.cpp
        3rdparty/imgui/imgui_draw.cpp
        3rdparty/imgui/imgui_tables.cpp
        3rdparty/imgui/imgui_widgets.cpp
        3rdparty/imgui/backends/imgui_impl_glfw.cpp
        3rdparty/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        3rdparty/imgui
        3rdparty/imgui/backends
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(imgui PUBLIC glfw glad)

add_executable(LearnOpenGL
        src/main.cpp
        src/renderer.cpp
        src/renderer.h
        src/vertex_buffer.cpp
        src/vertex_buffer.h
        src/index_buffer.cpp
        src/index_buffer.h
        src/vertex_buffer_layout.h
        src/vertex_array.cpp
        src/vertex_array.h
        src/shader.cpp
        src/shader.h
        3rdparty/stb/stb_image.cpp
        3rdparty/stb/stb_image.h
        src/texture.cpp
        src/texture.h
)

target_include_directories(LearnOpenGL PRIVATE 3rdparty/glfw/include 3rdparty/glad/include)
target_include_directories(glad PRIVATE 3rdparty/glad/include 3rdparty/glad/src)
target_include_directories(LearnOpenGL PRIVATE 3rdparty/glm)
target_include_directories(LearnOpenGL PRIVATE 3rdparty/soil/include)
target_include_directories(LearnOpenGL PRIVATE 3rdparty/stb)

target_link_libraries(LearnOpenGL PRIVATE glad glfw soil imgui)

if(APPLE)
    target_link_libraries(LearnOpenGL PRIVATE "-framework OpenGL")
endif()
