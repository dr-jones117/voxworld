#pragma once
#include <glm/glm.hpp>

#include "block.h"
#include "world/world.h"

typedef struct
{
    World &world;
    glm::vec3 origin;
    glm::vec3 rayDir;
    float maxDistance;
    void (*func)(glm::ivec3 &blockPos, char &face);
} RayCastInfo;

bool shoot_ray(RayCastInfo &info);

void doNothingIfHit(glm::ivec3 &blockPos, char &face);

char determineHitFace(const glm::vec3 &rayDir, const glm::vec3 &hitPoint);
