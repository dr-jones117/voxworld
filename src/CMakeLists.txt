add_executable(voxwrld main.cpp shader.cpp glError.cpp stb_image.cpp texture.cpp)

set_target_properties(voxwrld PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

target_link_libraries(voxwrld PRIVATE glfw glad OpenGL::GL glm::glm-header-only)

target_include_directories(voxwrld PRIVATE ${CMAKE_SOURCE_DIR}/lib/linmath 
	${VOXWRLD_SOURCE_DIR}/include)

# Define where the resources are and where they should be copied
set(RES_DIR ${CMAKE_SOURCE_DIR}/res)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/res)

# Copy the resource directory to the build output
add_custom_command(
    TARGET voxwrld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RES_DIR} ${OUTPUT_DIR}
)

