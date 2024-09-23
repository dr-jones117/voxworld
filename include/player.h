#pragma once

#include "camera.h"
#include "world/chunkData.h"
#include "physics.h"

class Player
{
private:
    Camera *camera;
    World *world;
    bool physicsEnabled;

public:
    Player(World *worldPtr)
    {
        camera = new Camera(1280, 720, worldPtr);
        world = worldPtr;
    }

    glm::vec3 getPos()
    {
        return camera->getPos();
    }

    glm::vec3 getFront()
    {
        return camera->getFront();
    }

    ChunkPos getChunkPos()
    {
        auto pos = camera->getPos();
        return {(int)(pos.x / CHUNK_SIZE), (int)(pos.z / CHUNK_SIZE)};
    }

    glm::mat4 getView()
    {
        return camera->getView();
    }

    void tick(float time)
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

    void setUpward(bool setter)
    {
        camera->setUpward(setter);
    }

    void setDownward(bool setter)
    {
        camera->setDownward(setter);
    }

    void setSpeedMode(bool setter)
    {
        camera->setSpeedMode(setter);
    }

    void updateLookCoords(double x, double y)
    {
        camera->updateLookCoords(x, y);
    }

    void setPhysics(bool setter)
    {
        camera->setPhysics(setter);
    }
};