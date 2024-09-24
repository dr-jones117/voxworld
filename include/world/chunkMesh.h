#pragma once

#include <vector>

#include "world/chunkData.h"
#include "world/chunkPos.h"

#include "glError.h"
#include "shader.h"
#include "PerlinNoise.hpp"
#include "rendering.h"

extern int render_distance;

const siv::PerlinNoise::seed_type seed = 439512u;
const siv::PerlinNoise perlin{seed};

#include "vec3.h"

typedef struct
{
    ChunkPos pos;
    unsigned int VBO, EBO, VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool isInitialized;
} ChunkMesh;

typedef std::unordered_map<ChunkPos, ChunkMesh, ChunkPosHash, ChunkPosEqual> ChunkMeshMap;
