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

Chunk *generateChunk(glm::ivec3 currPos)
{
    Chunk *chunk = new Chunk();
    chunk->pos = glm::vec3((float)currPos.x, (float)currPos.y, (float)currPos.z);

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

void renderChunk(Chunk *chunk, Shader *shader)
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_HEIGHT; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                if (chunk->data[i + CHUNK_SIZE * (j + CHUNK_HEIGHT * k)] == (BLOCK_TYPE)AIR_BLOCK)
                    continue;

                Texture *botT = textures[chunk->data[i + CHUNK_SIZE * (j + CHUNK_HEIGHT * k)] + BOTTOM_TEXTURE];
                Texture *sideT = textures[chunk->data[i + CHUNK_SIZE * (j + CHUNK_HEIGHT * k)] + SIDE_TEXTURE];
                Texture *topT = textures[chunk->data[i + CHUNK_SIZE * (j + CHUNK_HEIGHT * k)] + TOP_TEXTURE];

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((chunk->pos.x * (float)CHUNK_SIZE) + (float)i,
                                                        (chunk->pos.y * (float)CHUNK_HEIGHT) + (float)j,
                                                        (chunk->pos.z * (float)CHUNK_SIZE) + (float)k));

                unsigned int modelLoc = glGetUniformLocation(shader->Id, "model");
                GLCall(glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)));

                sideT->bind();
                GLCall(glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void *)0));

                botT->bind();
                GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float))));

                topT->bind();
                GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float))));
            }
        }
    }
}

void insertChunk(ChunkMap &chunkMap, Chunk *chunk, glm::ivec3 pos)
{
    if (chunkMap.find(pos) == chunkMap.end())
    {
        chunkMap[pos] = chunk;
    }
}

void generateChunks(glm::ivec3 startPos, ChunkMap &chunkMap)
{
    int x = startPos.x;
    int y = 0;
    int z = startPos.z;

    glm::ivec3 currPos = glm::ivec3(x, y, z);
    Chunk *currChunk = generateChunk(currPos);
    insertChunk(chunkMap, currChunk, currPos);

    for (int i = 1; i <= render_distance; i++)
    {
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x - i + j, y, z + i);
            Chunk *currChunk = generateChunk(currPos);
            insertChunk(chunkMap, currChunk, currPos);
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x + i, y, z + i - j);
            Chunk *currChunk = generateChunk(currPos);
            insertChunk(chunkMap, currChunk, currPos);
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x + i - j, y, z - i);
            Chunk *currChunk = generateChunk(currPos);
            insertChunk(chunkMap, currChunk, currPos);
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x - i, y, z - i + j);
            Chunk *currChunk = generateChunk(currPos);
            insertChunk(chunkMap, currChunk, currPos);
        }
    }
}
