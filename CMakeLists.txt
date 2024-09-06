cmake_minimum_required(VERSION 3.10)

project(VOXWRLD VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/glew)
add_subdirectory(src)
