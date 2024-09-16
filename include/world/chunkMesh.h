#pragma once

#include "world/chunkData.h"
#include <glm/glm.hpp>
#include "glError.h"
#include "shader.h"
#include "PerlinNoise.hpp"
#include "rendering.h"
#include <vector>

extern int render_distance;

const siv::PerlinNoise::seed_type seed = 234516u;
const siv::PerlinNoise perlin{seed};

#include "vec3.h"

typedef struct
{
    int x, z;
} ChunkPos;

typedef struct
{
    ChunkPos pos;
    unsigned int VBO, EBO, VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
} ChunkMesh;

typedef std::unordered_map<ChunkPos, ChunkMesh> ChunkMeshMap;
