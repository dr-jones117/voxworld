#include "glError.h"
#include <GL/glew.h>
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl
                  << "Function: " << function << std::endl
                  << "File: " << file << std::endl
                  << "Line: " << line
                  << std::endl;
        return false;
    }
    return true;
}