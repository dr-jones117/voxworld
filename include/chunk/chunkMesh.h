#pragma once

#include "chunk/chunkData.h"
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

class ChunkMesh
{
public:
    glm::ivec3 pos;
    unsigned int VBO, EBO, VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

typedef std::unordered_map<glm::ivec3, ChunkMesh, Vec3Hash, Vec3Equal> ChunkMeshMap;

void bindChunk(ChunkMesh &chunkMesh);
void unbindChunk(ChunkMesh &chunkMesh);

void renderChunkMeshes(ChunkMeshMap &chunkMeshMap);

void generateChunkMeshes(ChunkMeshMap &chunkMeshMap, ChunkDataMap &chunkDataMap, glm::ivec2 startPos);
void generateChunkMesh(ChunkMeshMap &chunkMeshMap, ChunkDataMap &chunkDataMap, glm::ivec3 currPos);

bool chunkMeshExists(ChunkMeshMap &chunkMeshMap, glm::ivec3 pos);
ChunkMesh *getChunkFromMap(ChunkMeshMap &chunkMeshMap, glm::ivec3 pos);
void removeChunkFromMap(ChunkMeshMap &chunkMeshMap, glm::ivec3 pos);
void removeUnneededChunks(ChunkMeshMap &chunkMeshMap, glm::ivec3 startPos);
