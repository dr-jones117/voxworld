#include "physics.h"

void doNothingIfHit(BLOCK block, glm::ivec3 &blockPos, char &face)
{
}

char determineHitFace(const glm::vec3 &rayDir, const glm::vec3 &hitPoint)
{
    float tolerance = 0.001;
    if (std::abs(std::floor(hitPoint.x) - hitPoint.x) < tolerance)
    {
        return rayDir.x > 0 ? 4 : 8;
    }
    else if (std::abs(std::floor(hitPoint.y) - hitPoint.y) < tolerance)
    {
        return rayDir.y > 0 ? 16 : 32;
    }
    else if (std::abs(std::floor(hitPoint.z) - hitPoint.z) < tolerance)
    {
        return rayDir.z > 0 ? 1 : 2;
    }
    return 0;
}

// Main DDA raycasting function
bool shoot_ray(RayCastInfo &info)
{
    glm::ivec3 blockPos = glm::ivec3(glm::floor(info.origin)); // Initial block position

    // Calculate the step direction for each axis
    glm::ivec3 step;
    step.x = (info.rayDir.x > 0) ? 1 : -1;
    step.y = (info.rayDir.y > 0) ? 1 : -1;
    step.z = (info.rayDir.z > 0) ? 1 : -1;

    // Calculate the distances to the first voxel boundary
    glm::vec3 tMax;
    tMax.x = (step.x > 0) ? (glm::floor(info.origin.x + 1) - info.origin.x) / info.rayDir.x : (info.origin.x - glm::floor(info.origin.x)) / -info.rayDir.x;
    tMax.y = (step.y > 0) ? (glm::floor(info.origin.y + 1) - info.origin.y) / info.rayDir.y : (info.origin.y - glm::floor(info.origin.y)) / -info.rayDir.y;
    tMax.z = (step.z > 0) ? (glm::floor(info.origin.z + 1) - info.origin.z) / info.rayDir.z : (info.origin.z - glm::floor(info.origin.z)) / -info.rayDir.z;

    // Calculate how far to step along the ray for each axis
    glm::vec3 tDelta;
    tDelta.x = glm::abs(1.0f / info.rayDir.x);
    tDelta.y = glm::abs(1.0f / info.rayDir.y);
    tDelta.z = glm::abs(1.0f / info.rayDir.z);

    float distanceTraveled = 0.0f;

    // Traverse through the blocks along the ray
    while (distanceTraveled <= info.maxDistance)
    {
        // Check the block data at the current position
        BLOCK block = info.world.getBlockData(blockPos);
        if (block != BLOCK::AIR_BLOCK)
        {
            // Calculate the exact intersection point
            glm::vec3 intersectionPoint = info.origin + info.rayDir * distanceTraveled;

            // Determine the face using the intersection point
            char face = determineHitFace(info.rayDir, intersectionPoint);
            info.func(block, blockPos, face);
            return true; // Exit if a block is hit
        }

        // Move to the next voxel boundary in the smallest tMax value direction
        if (tMax.x < tMax.y)
        {
            if (tMax.x < tMax.z)
            {
                blockPos.x += step.x;
                distanceTraveled = tMax.x;
                tMax.x += tDelta.x;
            }
            else
            {
                blockPos.z += step.z;
                distanceTraveled = tMax.z;
                tMax.z += tDelta.z;
            }
        }
        else
        {
            if (tMax.y < tMax.z)
            {
                blockPos.y += step.y;
                distanceTraveled = tMax.y;
                tMax.y += tDelta.y;
            }
            else
            {
                blockPos.z += step.z;
                distanceTraveled = tMax.z;
                tMax.z += tDelta.z;
            }
        }
    }

    return false;
}