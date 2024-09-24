#pragma once

#include <vector>

#include "world/chunkData.h"
#include "world/chunkPos.h"

#include "glError.h"
#include "shader.h"

#include "rendering.h"

extern int render_distance;

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
