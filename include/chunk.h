#pragma once

#include <glm/glm.hpp>
#include "glError.h"
#include "shader.h"
#include <vector>

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 64
#define RENDER_DISTANCE 1

typedef struct
{
    glm::vec3 pos;
    char data[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
} Chunk;

Chunk *generateChunk(glm::vec3 pos);
void renderChunk(Chunk *chunk, Shader *shader);

void generateChunks(glm::ivec3 currPos, std::vector<Chunk *> &chunks);