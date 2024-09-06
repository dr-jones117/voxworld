#pragma once
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderProgramSource {
    std::string vertexShader;
    std::string fragmentShader;
};

ShaderProgramSource ParseShader(const std::string& filepath);
unsigned int CompileShader(unsigned int type, const std::string& source);
unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);