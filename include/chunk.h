#pragma once

#include <glm/glm.hpp>
#include "glError.h"
#include "shader.h"

#define CHUNK_SIZE 16

typedef struct
{
    glm::vec3 pos;
    char data[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
} Chunk;

Chunk *generateChunk(glm::vec3 pos);
void renderChunk(Chunk *chunk, Shader *shader);
