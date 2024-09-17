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

#include "shader.h"
#include "glError.h"
#include "texture.h"
#include "player.h"

#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "world/chunkPos.h"
#include "world/world.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <rendering.h>

#define SHOW_FPS false

int screenWidth = 1280, screenHeight = 720;
Player *player = new Player();
World *world = new World();

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

void removeBlock(glm::ivec3 &pos, char &face)
{
    world->removeBlock(pos);
}

void focusBlock(glm::ivec3 &pos, char &face)
{
    world->updateFocusBlock(pos, face);
}

void shoot_ray(void (*func)(glm::ivec3 &blockPos, char &face))
{
    auto fVec = player->getFront(); // Direction of the ray
    auto pos = player->getPos();    // Origin of the ray

    float stepSize = 0.01f;   // Define the step size (distance between checks)
    float maxDistance = 5.0f; // Define the maximum distance to check

    // Iterate along the ray direction in steps
    for (float distance = 0.0f; distance <= maxDistance; distance += stepSize)
    {
        // Compute the current position along the ray
        glm::vec3 currentPos = pos + fVec * distance;

        // Cast to integer coordinates, rounding towards the origin
        glm::ivec3 intPos = glm::ivec3(glm::floor(currentPos.x), glm::floor(currentPos.y), glm::floor(currentPos.z));

        // Get block data at integer position
        BLOCK block = world->getBlockData(intPos);
        if (block != BLOCK::AIR_BLOCK)
        {
            // Calculate the hit face based on the ray direction and voxel position
            glm::vec3 intPosVec = glm::vec3(intPos);
            glm::vec3 hitPoint = currentPos - intPosVec;
            std::cout << "hitPoint: " << hitPoint.x << ", " << hitPoint.y << ", " << hitPoint.z << std::endl;

            float xDist = glm::abs(hitPoint.x);
            float yDist = glm::abs(hitPoint.y);
            float zDist = glm::abs(hitPoint.z);

            char face = 0;
            // Determine the hit face with a threshold to handle edge cases
            if (hitPoint.x > 0.99f)
            {
                face = 8;
            }
            else if (hitPoint.y > 0.99f)
            {
                face = 32;
            }
            else if (hitPoint.z > 0.99f)
            {
                face = 2;
            }
            else if (hitPoint.x == std::min(std::min(hitPoint.x, hitPoint.y), hitPoint.z))
            {
                face = 4;
            }
            else if (hitPoint.y == std::min(std::min(hitPoint.x, hitPoint.y), hitPoint.z))
            {
                face = 16;
            }
            else if (hitPoint.z == std::min(std::min(hitPoint.x, hitPoint.y), hitPoint.z))
            {
                face = 1;
            }

            func(intPos, face);
            break; // Exit the loop after detecting the block
        }
    }

    std::cout << "\n\n";
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        shoot_ray(removeBlock);
    }
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
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    Shader shader = Shader("./res/shaders/object.shader");
    shader.useProgram();

    Texture atlas = Texture("./res/textures/texture-atlas.jpg", GL_REPEAT, GL_NEAREST);
    atlas.bind();

    world->init();

    // Timing variables
    auto startTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    float fps = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        frameCount++;

        ChunkPos playerChunkPos = player->getChunkPos();
        world->generateNewChunks(playerChunkPos);

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

        glm::mat4 model;
        model = glm::mat4(1.0f);
        int modelLoc = glGetUniformLocation(shader.Id, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        shoot_ray(focusBlock);
        world->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Calculate FPS
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - startTime;

        glm::vec3 playerPos = player->getPos();
        // std::cout << "Chunk: (" << playerPos.x << ", " << playerPos.y << ", " << playerPos.z << ") " << std::endl;
#if SHOW_FPS
        if (elapsed.count() >= 1.0f)
        {
            fps = frameCount / elapsed.count();
            std::cout << "FPS: " << fps << std::endl;
            startTime = currentTime;
            frameCount = 0;
        }
#endif
    }

    shader.deleteProgram();

    glfwDestroyWindow(window);
    delete player;
    delete world;

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
