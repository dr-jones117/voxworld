#pragma once
#include <string>

#include "glError.h"

struct ShaderProgramSource
{
    std::string vertexShader;
    std::string fragmentShader;
};

class Shader
{
private:
    ShaderProgramSource ParseShader(const std::string &filepath);
    unsigned int CompileShader(unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

public:
    unsigned int Id;
    Shader(std::string filepath);
    void useProgram();
    void deleteProgram();
};