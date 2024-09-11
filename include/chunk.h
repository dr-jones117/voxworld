#pragma once

#include <glm/glm.hpp>
#include "glError.h"
#include "shader.h"
#include "PerlinNoise.hpp"
#include "rendering.h"
#include <vector>

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 32
extern int render_distance;

const siv::PerlinNoise::seed_type seed = 123456u;
const siv::PerlinNoise perlin{seed};

#include "vec3.h"

typedef struct
{
    glm::ivec3 pos;
    char data[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VBO, EBO, VAO;
} Chunk;

typedef std::unordered_map<glm::ivec3, Chunk *, Vec3Hash, Vec3Equal> ChunkMap;

Chunk *generateChunk(int x, int y, int z);
void renderChunks(ChunkMap *chunkMap, Shader *shader);
void bindChunk(Chunk *chunk);

void generateChunks(glm::ivec3 currPos, ChunkMap &chunks);