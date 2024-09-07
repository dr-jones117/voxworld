#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>

#include "shader.h"
#include "glError.h"

#define SHOW_CWD false

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
}

int main(void)
{
#if SHOW_CWD
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr)
    {
        std::cout << "Current working directory: " << cwd << std::endl;
    }
    else
    {
        perror("getcwd() error");
    }
#endif

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "OpenGL Triangle", NULL, NULL);
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "error while initializing glew\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f,
        -0.5f,
        0.0f,

        0.5f,
        0.2f,
        0.2f,

        0.5f,
        -0.5f,
        0.0f,

        0.0f,
        1.0f,
        0.0f,
        0.5f,
        0.5f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,

        -0.7f,
        -0.7f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        -0.5f,
        -0.7f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        -0.6f,
        0.7f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5};

    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    unsigned int VBO;
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));

    unsigned int EBO;
    GLCall(glGenBuffers(1, &EBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    ShaderProgramSource blueShaderSource = ParseShader("../res/shaders/Blue.shader");
    unsigned int blueShader = CreateShader(blueShaderSource.vertexShader, blueShaderSource.fragmentShader);

    ShaderProgramSource uniformShaderSource = ParseShader("../res/shaders/UniformTest.shader");
    unsigned int uniformShader = CreateShader(uniformShaderSource.vertexShader, uniformShaderSource.fragmentShader);

    while (!glfwWindowShouldClose(window))
    {
        float timeValue = glfwGetTime();
        float redValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(uniformShader, "red");
        glUseProgram(uniformShader);
        glUniform1f(vertexColorLocation, redValue); // need to call use program before this so it assigns the value to the correct shader

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glClearColor(0.0f, 0.9f, 1.0f, 1.0f));

        GLCall(glUseProgram(blueShader));
        int blueOffset = glGetUniformLocation(blueShader, "offset");
        glUniform1f(blueOffset, redValue > 1.0f ? 1.0f : redValue);

        GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)0));

        GLCall(glUseProgram(uniformShader));
        GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(3 * sizeof(unsigned int))));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(blueShader);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
