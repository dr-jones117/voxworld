#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"

Camera *currentCam = nullptr;
std::vector<Camera *> cameras;
int currentCamIdx = 0;
int curr_cam_id = 1;

void setCurrentCamera(Camera *camera)
{
    if (currentCam)
    {
        currentCam->setForward(false);
        currentCam->setBackward(false);
        currentCam->setLeftward(false);
        currentCam->setRightward(false);
        currentCam->setSpeedMode(false);
    }

    currentCam = camera;
    std::cout << "Current Camera: " << currentCam->Id << std::endl;
}

void toggleCurrentCamera()
{
    currentCamIdx = ++currentCamIdx % cameras.size();
    setCurrentCamera(cameras.at(currentCamIdx));
}

Camera::Camera(int screenwidth, int screenHeight)
{
    lastX = screenwidth / 2;
    lastY = screenHeight / 2;

    float deltaTime = 0.0f;
    float lastTick = 0.0f;

    speedMode = false;

    Id = curr_cam_id;
    curr_cam_id++;

    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    firstLook = true;
    yaw = 0.0f;
    pitch = 0.0f;

    baseSpeed = 12.0f;
}

glm::mat4 Camera::getView()
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}

void Camera::tick(double currentTime)
{
    speed = baseSpeed * deltaTime;
    if (speedMode)
    {
        speed *= 3.0f;
    }

    if (movingForward)
    {
        cameraPos += speed * cameraFront;
    }
    if (movingBackward)
    {
        cameraPos -= speed * cameraFront;
    }
    if (movingLeftward)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    }
    if (movingRightward)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    }

    deltaTime = currentTime - lastTick;
    lastTick = currentTime;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void Camera::setForward(bool setter)
{
    movingForward = setter;
}

void Camera::setBackward(bool setter)
{
    movingBackward = setter;
}

void Camera::setLeftward(bool setter)
{
    movingLeftward = setter;
}

void Camera::setRightward(bool setter)
{
    movingRightward = setter;
}

void Camera::setSpeedMode(bool setter)
{
    speedMode = setter;
}

void Camera::updateLookCoords(double xpos, double ypos)
{
    if (firstLook)
    {
        lastX = xpos;
        lastY = ypos;
        firstLook = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
}

glm::vec3 Camera::getPos()
{
    return cameraPos;
}