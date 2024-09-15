#pragma once

#include "camera.h"
#include "chunk/chunkData.h"

class Player
{
private:
    Camera *camera;

public:
    Player()
    {
        camera = new Camera(1280, 720);
    }

    glm::vec3 getPos()
    {
        return camera->getPos();
    }

    glm::ivec2 getChunkPos()
    {
        auto pos = camera->getPos();
        return glm::ivec2((int)(pos.x / CHUNK_SIZE), (int)(pos.z / CHUNK_SIZE));
    }

    glm::mat4 getView()
    {
        return camera->getView();
    }

    void tick(double time)
    {
        camera->tick(time);
    }

    void setForward(bool setter)
    {
        camera->setForward(setter);
    }

    void setBackward(bool setter)
    {
        camera->setBackward(setter);
    }

    void setLeftward(bool setter)
    {
        camera->setLeftward(setter);
    }

    void setRightward(bool setter)
    {
        camera->setRightward(setter);
    }

    void setSpeedMode(bool setter)
    {
        camera->setSpeedMode(setter);
    }

    void updateLookCoords(double x, double y)
    {
        camera->updateLookCoords(x, y);
    }
};