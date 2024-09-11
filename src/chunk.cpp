#include <glad/glad.h>

#include "chunk.h"
#include "voxwrld.h"
#include "glError.h"
#include "PerlinNoise.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

int render_distance = 2;

void bindChunk(Chunk *chunk)
{
    GLCall(glBindVertexArray(chunk->VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO));
}

void unbindChunk(Chunk *chunk)
{
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

Chunk *generateChunk(glm::ivec3 currPos)
{
    Chunk *chunk = new Chunk();
    chunk->pos = glm::vec3((float)currPos.x, (float)currPos.y, (float)currPos.z);

    GLCall(glGenVertexArrays(1, &chunk->VAO));
    GLCall(glGenBuffers(1, &chunk->VBO));
    GLCall(glGenBuffers(1, &chunk->EBO));

    bindChunk(chunk);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(glm::vec3)));
    GLCall(glEnableVertexAttribArray(1));

    unbindChunk(chunk);

    unsigned int indiceOffset = 0;

    for (int i = 0; i < CHUNK_SIZE; i++) // X-axis
    {
        for (int k = 0; k < CHUNK_SIZE; k++) // Z-axis
        {
            double freq = 0.01;
            double noise = perlin.octave2D_01(freq * (currPos.x * CHUNK_SIZE + i), freq * (currPos.z * CHUNK_SIZE + k), 8);

            int terrainHeight = (int)(noise * CHUNK_HEIGHT);

            for (int j = 0; j < CHUNK_HEIGHT; j++) // Y-axis
            {
                if (j < terrainHeight)
                {
                    chunk->data[i + CHUNK_SIZE * (j + CHUNK_HEIGHT * k)] = (BLOCK_TYPE)GRASS_BLOCK;

                    // World position of the block
                    glm::vec3 blockPos = glm::vec3((currPos.x * CHUNK_SIZE) + i, (currPos.y * CHUNK_SIZE) + j, (currPos.z * CHUNK_SIZE) + k);

                    // North face
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)});

                    chunk->indices.push_back(indiceOffset + 0);
                    chunk->indices.push_back(indiceOffset + 1);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 3);
                    chunk->indices.push_back(indiceOffset + 0);

                    indiceOffset += 4;

                    // South face
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)});

                    chunk->indices.push_back(indiceOffset + 0);
                    chunk->indices.push_back(indiceOffset + 1);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 3);
                    chunk->indices.push_back(indiceOffset + 0);

                    indiceOffset += 4;

                    // West face
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)});

                    chunk->indices.push_back(indiceOffset + 0);
                    chunk->indices.push_back(indiceOffset + 1);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 3);
                    chunk->indices.push_back(indiceOffset + 0);

                    indiceOffset += 4;

                    // East face
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)});

                    chunk->indices.push_back(indiceOffset + 0);
                    chunk->indices.push_back(indiceOffset + 1);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 3);
                    chunk->indices.push_back(indiceOffset + 0);

                    indiceOffset += 4;

                    // Bottom face
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)});

                    chunk->indices.push_back(indiceOffset + 0);
                    chunk->indices.push_back(indiceOffset + 1);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 3);
                    chunk->indices.push_back(indiceOffset + 0);

                    indiceOffset += 4;

                    // Top face
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f)});
                    chunk->vertices.push_back({blockPos + glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f)});

                    chunk->indices.push_back(indiceOffset + 0);
                    chunk->indices.push_back(indiceOffset + 1);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 2);
                    chunk->indices.push_back(indiceOffset + 3);
                    chunk->indices.push_back(indiceOffset + 0);

                    indiceOffset += 4;
                }
                else
                {
                    chunk->data[i + CHUNK_SIZE * (j + CHUNK_HEIGHT * k)] = (BLOCK_TYPE)AIR_BLOCK;
                }
            }
        }
    }
    return chunk;
}

void renderChunks(ChunkMap *chunkMap, Shader *shader)
{
    for (const auto &pair : *chunkMap)
    {
        Chunk *chunk = pair.second;
        bindChunk(chunk);

        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * chunk->vertices.size(), &chunk->vertices.front(), GL_STATIC_DRAW));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * chunk->indices.size(), &chunk->indices.front(), GL_STATIC_DRAW));
        GLCall(glDrawElements(GL_TRIANGLES, chunk->indices.size(), GL_UNSIGNED_INT, (void *)0));

        unbindChunk(chunk);
    }
}

bool chunkExists(ChunkMap &chunkMap, glm::ivec3 pos)
{
    if (chunkMap.find(pos) == chunkMap.end())
        return false;
    return true;
}

void generateChunks(glm::ivec3 startPos, ChunkMap &chunkMap)
{
    int x = startPos.x;
    int y = 0;
    int z = startPos.z;

    glm::ivec3 currPos = glm::ivec3(x, y, z);
    if (!chunkExists(chunkMap, currPos))
    {
        Chunk *currChunk = generateChunk(currPos);
        chunkMap[currPos] = currChunk;
    }

    for (int i = 1; i <= render_distance; i++)
    {
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x - i + j, y, z + i);
            if (!chunkExists(chunkMap, currPos))
            {
                Chunk *currChunk = generateChunk(currPos);
                chunkMap[currPos] = currChunk;
            }
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x + i, y, z + i - j);
            if (!chunkExists(chunkMap, currPos))
            {
                Chunk *currChunk = generateChunk(currPos);
                chunkMap[currPos] = currChunk;
            }
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x + i - j, y, z - i);
            if (!chunkExists(chunkMap, currPos))
            {
                Chunk *currChunk = generateChunk(currPos);
                chunkMap[currPos] = currChunk;
            }
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x - i, y, z - i + j);
            if (!chunkExists(chunkMap, currPos))
            {
                Chunk *currChunk = generateChunk(currPos);
                chunkMap[currPos] = currChunk;
            }
        }
    }
}
