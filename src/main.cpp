#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unistd.h>
#include <limits.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

struct ShaderProgramSource {
    std::string vertexShader;
    std::string fragmentShader;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;

    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return { "", "" };
    }

    std::string line;
    std::stringstream ss[2];

    while(getline(stream, line)) {
        if(line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << std::endl;
        }
    }

    return { ss[0].str(), ss[1].str() };
}
 
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        int length;

        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile shader..." << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
 
int main(void)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }

    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "error while initializing window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
 
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "error while initializing glew\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwSwapInterval(1);

    float positions[12] = {
        -0.5f,  -0.5f, 
         0.5f,  -0.5f, 
         0.5f,   0.5f,

         0.5f,  0.5f, 
         -0.5f,  0.5f, 
         -0.5f,  -0.5f
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    // read in the shader here
    ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.vertexShader, source.fragmentShader);

    std::cout << source.vertexShader << std::endl;
    std::cout << source.fragmentShader << std::endl;

    glUseProgram(shader);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
