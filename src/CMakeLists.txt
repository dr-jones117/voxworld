add_executable(voxwrld main.cpp shader.cpp glError.cpp stb_image.cpp texture.cpp)

set_target_properties(voxwrld PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

find_package(glm CONFIG REQUIRED)

target_link_libraries(voxwrld PRIVATE glfw glad glew OpenGL::GL glm::glm-header-only)

target_include_directories(voxwrld PRIVATE ${CMAKE_SOURCE_DIR}/lib/linmath 
	${VOXWRLD_SOURCE_DIR}/include)


