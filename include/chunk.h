#pragma once

#include <glm/glm.hpp>
#include "glError.h"
#include "shader.h"
#include <vector>

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 12
extern int render_distance;

typedef struct
{
    glm::vec3 pos;
    char data[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
} Chunk;

Chunk *generateChunk(int x, int y, int z);
void renderChunk(Chunk *chunk, Shader *shader);

void generateChunks(glm::ivec3 currPos, std::vector<Chunk *> &chunks);