#pragma once

#include <vector>

#include "world/chunkData.h"
#include "world/chunkPos.h"

#include "glError.h"
#include "shader.h"
#include "PerlinNoise.hpp"
#include "rendering.h"

extern int render_distance;

const siv::PerlinNoise::seed_type seed = 939512u;
const siv::PerlinNoise perlin{seed};

#include "vec3.h"

typedef struct
{
    ChunkPos pos;
    unsigned int VBO_opaque, EBO_opaque, VAO_opaque;
    unsigned int VBO_transparent, EBO_transparent, VAO_transparent;

    std::vector<Vertex> vertices_opaque;
    std::vector<unsigned int> indices_opaque;

    std::vector<Vertex> vertices_transparent;
    std::vector<unsigned int> indices_transparent;

    bool isInitialized;
    bool transparentInitialized;
} ChunkMesh;

typedef std::unordered_map<ChunkPos, ChunkMesh, ChunkPosHash, ChunkPosEqual> ChunkMeshMap;
