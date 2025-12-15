cmake_minimum_required(VERSION 4.0)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(ThirdParty/glfw)
add_subdirectory(ThirdParty/soil)

add_library(glad STATIC ThirdParty/glad/src/glad.c)

add_executable(LearnOpenGL
        Main.cpp
)

target_include_directories(LearnOpenGL PRIVATE ThirdParty/glfw/include ThirdParty/glad/include)
target_include_directories(glad PRIVATE ThirdParty/glad/include ThirdParty/glad/src)
target_include_directories(LearnOpenGL PRIVATE ThirdParty/glm)
target_include_directories(LearnOpenGL PRIVATE ThirdParty/soil/include)
target_link_libraries(LearnOpenGL PRIVATE glad glfw soil)

if(APPLE)
    target_link_libraries(LearnOpenGL PRIVATE "-framework OpenGL")
endif()
