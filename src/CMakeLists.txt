add_executable(voxwrld main.cpp shader.cpp glError.cpp stb_image.cpp texture.cpp camera.cpp block.cpp physics.cpp threading.cpp world/chunkData.cpp world/chunkMesh.cpp world/world.cpp)

set_target_properties(voxwrld PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

target_link_libraries(voxwrld PRIVATE glfw glad OpenGL::GL glm::glm-header-only)

target_include_directories(voxwrld PRIVATE ${CMAKE_SOURCE_DIR}/lib/PerlinNoise ${CMAKE_SOURCE_DIR}/lib/imgui ${CMAKE_SOURCE_DIR}/lib/imgui/backends 
	${VOXWRLD_SOURCE_DIR}/include)


set(RES_DIR ${CMAKE_SOURCE_DIR}/res)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/res)


add_custom_command(
    TARGET voxwrld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RES_DIR} ${OUTPUT_DIR}
)

# Define ImGui source files
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/lib/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

# Add the ImGui backend for the technology you use (adjust as necessary)
set(IMGUI_BACKENDS
    ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp  # Assuming you're using GLFW
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp  # Assuming you're using OpenGL 3
)

# Add ImGui source files to the target
target_sources(voxwrld PRIVATE ${IMGUI_SOURCES} ${IMGUI_BACKENDS})

# Add ImGui backends header files to include directories
target_include_directories(voxwrld PRIVATE ${IMGUI_BACKENDS_DIR})

