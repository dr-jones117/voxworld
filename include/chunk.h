#pragma once

#include <glm/glm.hpp>
#include "glError.h"
#include "shader.h"
#include "PerlinNoise.hpp"
#include <vector>

#define CHUNK_SIZE 12
#define CHUNK_HEIGHT 12
extern int render_distance;

const siv::PerlinNoise::seed_type seed = 123456u;
const siv::PerlinNoise perlin{seed};

#include "vec3.h"

typedef struct
{
    glm::vec3 pos;
    char data[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
} Chunk;

typedef std::unordered_map<glm::ivec3, Chunk *, Vec3Hash, Vec3Equal> ChunkMap;

Chunk *generateChunk(int x, int y, int z);
void renderChunk(Chunk *chunk, Shader *shader);

void generateChunks(glm::ivec3 currPos, ChunkMap &chunks);