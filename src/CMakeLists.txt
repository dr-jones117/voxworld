add_executable(voxwrld main.cpp)

target_link_libraries(voxwrld PRIVATE glfw glad glew OpenGL::GL)

target_include_directories(voxwrld PRIVATE ${CMAKE_SOURCE_DIR}/lib/linmath 
	${VOXWRLD_SOURCE_DIR}/include)


