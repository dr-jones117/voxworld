#ifndef VEC3_H
#define VEC3_H

#include <functional>
#include <glm/glm.hpp>

struct Vec3Hash
{
    std::size_t operator()(const glm::ivec3 &v) const noexcept
    {
        return std::hash<float>()(v.x) ^ std::hash<float>()(v.y) ^ std::hash<float>()(v.z);
    }
};

struct Vec3Equal
{
    bool operator()(const glm::ivec3 &v1, const glm::ivec3 &v2) const
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
    }
};

struct Vec2Hash
{
    std::size_t operator()(const glm::ivec2 &v) const noexcept
    {
        return std::hash<float>()(v.x) ^ std::hash<float>()(v.y);
    }
};

struct Vec2Equal
{
    bool operator()(const glm::ivec2 &v1, const glm::ivec2 &v2) const
    {
        return v1.x == v2.x && v1.y == v2.y;
    }
};

#endif
