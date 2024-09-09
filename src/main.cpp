#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>

#include "voxwrld.h"
#include "shader.h"
#include "glError.h"
#include "texture.h"
#include "chunk.h"
#include "player.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

int screenWidth = 1280, screenHeight = 720;
Player *player = new Player();

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void togglePause(GLFWwindow *window)
{
    int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
    if (cursorMode == GLFW_CURSOR_DISABLED)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        togglePause(window);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player->setForward(true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        player->setForward(false);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player->setBackward(true);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        player->setBackward(false);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player->setLeftward(true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        player->setLeftward(false);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player->setRightward(true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        player->setRightward(false);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        player->setSpeedMode(true);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        player->setSpeedMode(false);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
    screenWidth = width;
    screenHeight = height;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    player->updateLookCoords(xpos, ypos);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "voxwrld", nullptr, nullptr);
    if (!window)
    {
        fprintf(stderr, "error while initializing window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f};

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20};

    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    unsigned int VBO;
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));

    unsigned int EBO;
    GLCall(glGenBuffers(1, &EBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    Shader shader = Shader("./res/shaders/object.shader");

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    shader.useProgram();

    // Timing variables
    auto startTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    float fps = 0.0f;

    generateTextures();

    std::vector<Chunk *> chunks;
    generateChunks(player->getChunkPos(), chunks);

    while (!glfwWindowShouldClose(window))
    {
        frameCount++;
        processInput(window);
        player->tick(glfwGetTime());

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glClearColor(0.2f, 0.65f, 1.0f, 1.0f));

        // view
        glm::mat4 view = player->getView();
        int viewLoc = glGetUniformLocation(shader.Id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // projection
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(65.0f), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        int projectionLoc = glGetUniformLocation(shader.Id, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLCall(glBindVertexArray(VAO));
        for (const auto &chunk : chunks)
        {
            renderChunk(chunk, &shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Calculate FPS
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - startTime;

        glm::vec3 chunkPos = player->getChunkPos();
        std::cout << "Chunk: (" << chunkPos.x << ", " << chunkPos.y << ", " << chunkPos.z << ") " << std::endl;

        if (elapsed.count() >= 1.0f)
        {
            fps = frameCount / elapsed.count();
            // std::cout << "FPS: " << fps << std::endl;
            startTime = currentTime;
            frameCount = 0;
        }
    }

    shader.deleteProgram();

    glfwDestroyWindow(window);
    delete player;

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
