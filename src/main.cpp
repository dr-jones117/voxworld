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
#include "physics.h"

#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "world/chunkPos.h"
#include "world/world.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <rendering.h>

#define SHOW_FPS true

int screenWidth = 1280, screenHeight = 720;
World *world = new World();
Player *player = new Player(world);

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

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        player->setDownward(true);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
        player->setDownward(false);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player->setUpward(true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        player->setUpward(false);

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

void playerHitBlock(BLOCK block, glm::ivec3 &pos, char &face)
{
    if (isLiquid(block))
        return;
    world->removeBlock(pos);
}

void focusBlock(BLOCK block, glm::ivec3 &pos, char &face)
{
    world->updateFocusBlock(pos, face);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        RayCastInfo info = {*world, player->getPos(), player->getFront(), 5.0f, playerHitBlock};
        shoot_ray(info);
    }
}

int main(void)
{
    player->setPhysics(false);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader = Shader("./res/shaders/object.shader");
    shader.useProgram();

    Texture atlas = Texture("./res/textures/atlas-new.png", GL_REPEAT, GL_NEAREST);
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

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glClearColor(0.2f, 0.65f, 1.0f, 1.0f));

        // view
        glm::mat4 view = player->getView();
        int viewLoc = glGetUniformLocation(shader.Id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // projection
        glm::mat4 projection;
        float radians = 75.0f;
        if (player->getSpeedMode())
        {
            radians = radians * 1.1;
        }
        projection = glm::perspective(glm::radians(radians), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        int projectionLoc = glGetUniformLocation(shader.Id, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model;
        model = glm::mat4(1.0f);
        int modelLoc = glGetUniformLocation(shader.Id, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        auto playerfront = player->getFront();
        // std::cout << "front: (" << playerfront.x << ", " << playerfront.y << ", " << playerfront.z << ") " << std::endl;

        player->tick(glfwGetTime());
        RayCastInfo info = {*world, player->getPos(), player->getFront(), 5.0f, focusBlock};
        shoot_ray(info);

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
