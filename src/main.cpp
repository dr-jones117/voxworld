#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "shader.h"

#define SHOW_CWD false


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
#if SHOW_CWD
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }
#endif

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
