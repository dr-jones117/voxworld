add_library(glew src/glew.c src/glewinfo.c src/visualinfo.c)

target_include_directories(glew PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
