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
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "error while initializing glew\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[8] = {
        -0.5f, -0.5f, // 0
        0.5f, -0.5f,  // 1
        0.5f, 0.5f,   // 2
        -0.5f, 0.5f,  // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    unsigned int VBO;
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    unsigned int EBO;
    GLCall(glGenBuffers(1, &EBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.vertexShader, source.fragmentShader);

    float r = 0.0f;
    float increment = 0.01f;

    while (!glfwWindowShouldClose(window))
    {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // default

        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        if (r >= 1.0f)
        {
            increment = -0.01f;
        }
        else if (r <= 0.0f)
        {
            increment = 0.01f;
        }

        r += increment;

        GLCall(glUseProgram(shader));
        int location = glGetUniformLocation(shader, "u_Color");
        GLCall(glUniform4f(location, r, 1.0f, 0.0f, 0.5f));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
