#pragma once

#include <glm/glm.hpp>
#include "glError.h"
#include "shader.h"
#include "PerlinNoise.hpp"
#include "rendering.h"
#include <vector>

#define CHUNK_SIZE 6
#define CHUNK_HEIGHT 6

extern int render_distance;

const siv::PerlinNoise::seed_type seed = 123456u;
const siv::PerlinNoise perlin{seed};

#include "vec3.h"

typedef struct
{
    glm::ivec3 pos;
    char data[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];

    unsigned int VBO, EBO, VAO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

} Chunk;

typedef std::unordered_map<glm::ivec3, Chunk, Vec3Hash, Vec3Equal> ChunkMap;

void bindChunk(Chunk &chunk);
void unbindChunk(Chunk &chunk);

void renderChunks(ChunkMap &chunkMap);

void generateChunks(ChunkMap &chunkMap, glm::ivec3 currPos);
void generateChunk(ChunkMap &chunkMap, glm::ivec3 currPos);

bool chunkExists(ChunkMap &chunkMap, glm::ivec3 pos);
Chunk *getChunkFromMap(ChunkMap &chunkMap, glm::ivec3 pos);
void removeChunkFromMap(ChunkMap &chunkMap, glm::ivec3 pos);
void removeUnneededChunks(ChunkMap &chunkMap, glm::ivec3 startPos);
