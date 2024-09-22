#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "world/world.h"

class Camera
{
private:
    float baseSpeed;
    bool speedMode;
    World *world;

    float speed;
    float maxSpeed;
    float height;

    bool isJumping;

    glm::vec3 velocity;
    glm::vec3 acceleration;

    float lastX, lastY;

    float deltaTime;
    float lastTick;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    bool firstLook;
    float yaw;
    float pitch;

    bool movingForward;
    bool movingBackward;
    bool movingLeftward;
    bool movingRightward;
    bool movingUpward;
    bool movingDownward;
    bool physics;

public:
    int Id;
    Camera(int screenWidth, int screenHeight, World *worldPtr);

    void tick(double currentTime);
    glm::mat4 getView();
    glm::vec3 getPos();
    glm::vec3 getFront();

    void setForward(bool setter);
    void setBackward(bool setter);
    void setLeftward(bool setter);
    void setRightward(bool setter);
    void setUpward(bool setter);
    void setDownward(bool setter);
    void setSpeedMode(bool setter);
    void setPhysics(bool setter);
    void updateLookCoords(double xpos, double ypos);
};

void setCurrentCamera(Camera *camera);
void toggleCurrentCamera();

extern Camera *currentCam;
extern std::vector<Camera *> cameras;
extern int currentCamIdx;
extern int curr_cam_id;