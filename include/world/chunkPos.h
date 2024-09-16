#pragma once

#include <functional>

typedef struct
{
    int x, z;
} ChunkPos;

struct ChunkPosHash
{
    std::size_t operator()(const ChunkPos &v) const noexcept
    {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.z);
    }
};

struct ChunkPosEqual
{
    bool operator()(const ChunkPos &v1, const ChunkPos &v2) const
    {
        return v1.x == v2.x && v1.z == v2.z;
    }
};