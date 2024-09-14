#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "chunk.h"
#include "glError.h"
#include "PerlinNoise.hpp"
#include "block.h"

int render_distance = 4;

void bindChunk(Chunk &chunk)
{
    GLCall(glBindVertexArray(chunk.VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, chunk.VBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.EBO));
}

void unbindChunk(Chunk &chunk)
{
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

std::vector<char> getChunkData(ChunkMap &chunkMap, glm::ivec3 pos)
{
    int blocksPerChunk = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE;
    std::vector<char> data;
    data.resize(blocksPerChunk);

    for (int i = 0; i < CHUNK_SIZE; i++) // X-axis
    {
        for (int k = 0; k < CHUNK_SIZE; k++) // Z-axis
        {
            double freq = 0.1;
            double noise = perlin.octave2D_01(freq * (pos.x * CHUNK_SIZE + i), freq * (pos.z * CHUNK_SIZE + k), 8);

            int terrainHeight = (int)(noise * CHUNK_HEIGHT);

            for (int j = 0; j < CHUNK_HEIGHT; j++) // Y-axis
            {
                if (j < terrainHeight)
                {
                    data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::GRASS_BLOCK;
                }
                else
                {
                    data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::AIR_BLOCK;
                }
            }
        }
    }

    return data;
}

void generateChunk(ChunkMap &chunkMap, glm::ivec3 currPos)
{
    Chunk chunk;
    chunk.pos = currPos;

    GLCall(glGenVertexArrays(1, &chunk.VAO));
    GLCall(glGenBuffers(1, &chunk.VBO));
    GLCall(glGenBuffers(1, &chunk.EBO));

    bindChunk(chunk);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(glm::vec3)));
    GLCall(glEnableVertexAttribArray(1));

    unbindChunk(chunk);

    unsigned int indiceOffset = 0;

    chunk.data = getChunkData(chunkMap, currPos);

    for (int x = 0; x < CHUNK_SIZE; x++) // X-axis
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++) // Z-axis
        {
            for (int z = 0; z < CHUNK_SIZE; z++) // Y-axis
            {
                BLOCK block = (BLOCK)chunk.data[x + y * CHUNK_SIZE + (z * CHUNK_SIZE * CHUNK_HEIGHT)];

                BlockRenderInfo renderInfo = {
                    (char)0,
                    glm::vec3((currPos.x * CHUNK_SIZE) + x, y, (currPos.z * CHUNK_SIZE) + z),
                    chunk.vertices,
                    chunk.indices,
                    indiceOffset,
                };

                // North face
                int northIndex = x + (y * CHUNK_SIZE) + ((z - 1) * CHUNK_SIZE * CHUNK_HEIGHT);
                if (z <= 0 || chunk.data[northIndex] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 1;
                }
                // South face
                int southIdx = x + (y * CHUNK_SIZE) + ((z + 1) * CHUNK_SIZE * CHUNK_HEIGHT);
                if (z >= CHUNK_SIZE - 1 || chunk.data[southIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 2;
                }
                // West face
                int westIdx = (x - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
                if (x <= 0 || chunk.data[westIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 4;
                }
                // East face
                int eastIdx = (x + 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
                if (x >= CHUNK_SIZE - 1 || chunk.data[eastIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 8;
                }
                // Bottom face
                int bottomIdx = x + ((y - 1) * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
                if (y <= 0 || chunk.data[bottomIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 16;
                }
                // Top face
                int topIdx = x + ((y + 1) * CHUNK_SIZE) + ((z)*CHUNK_SIZE * CHUNK_HEIGHT);
                if (y >= CHUNK_HEIGHT - 1 || chunk.data[topIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 32;
                }

                blockRenderFunctions[block](renderInfo);
            }
        }
    }

    chunkMap[currPos] = chunk;
}

void renderChunks(ChunkMap &chunkMap)
{
    for (const auto &pair : chunkMap)
    {
        Chunk chunk = pair.second;
        bindChunk(chunk);

        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * chunk.vertices.size(), &chunk.vertices.front(), GL_STATIC_DRAW));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * chunk.indices.size(), &chunk.indices.front(), GL_STATIC_DRAW));
        GLCall(glDrawElements(GL_TRIANGLES, chunk.indices.size(), GL_UNSIGNED_INT, (void *)0));

        unbindChunk(chunk);
    }
}

bool chunkExists(ChunkMap &chunkMap, glm::ivec3 pos)
{
    if (chunkMap.find(pos) == chunkMap.end())
        return false;
    return true;
}

void removeChunkFromMap(ChunkMap &chunkMap, glm::ivec3 pos)
{
    if (chunkExists(chunkMap, pos))
    {
        chunkMap.erase(pos);
    }
}

void removeUnneededChunks(ChunkMap &chunkMap, glm::ivec3 startPos)
{
    std::vector<glm::ivec3> chunkPosToRemove;
    for (const auto &pair : chunkMap)
    {
        Chunk chunk = pair.second;
        glm::vec3 vector = chunk.pos - startPos;
        if ((int)glm::length(vector) > (render_distance + 3))
        {
            chunkPosToRemove.push_back(chunk.pos);
        }
    }

    for (const auto &pos : chunkPosToRemove)
    {
        removeChunkFromMap(chunkMap, pos);
    }
}

void generateChunks(ChunkMap &chunkMap, glm::ivec3 startPos)
{
    int x = startPos.x;
    int y = 0;
    int z = startPos.z;

    glm::ivec3 currPos = glm::ivec3(x, y, z);
    if (!chunkExists(chunkMap, currPos))
    {
        generateChunk(chunkMap, currPos);
    }

    for (int i = 1; i <= render_distance; i++)
    {
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x - i + j, y, z + i);
            if (!chunkExists(chunkMap, currPos))
            {
                generateChunk(chunkMap, currPos);
            }
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x + i, y, z + i - j);
            if (!chunkExists(chunkMap, currPos))
            {
                generateChunk(chunkMap, currPos);
            }
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x + i - j, y, z - i);
            if (!chunkExists(chunkMap, currPos))
            {
                generateChunk(chunkMap, currPos);
            }
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec3(x - i, y, z - i + j);
            if (!chunkExists(chunkMap, currPos))
            {
                generateChunk(chunkMap, currPos);
            }
        }
    }

    removeUnneededChunks(chunkMap, startPos);
}

Chunk *getChunkFromMap(ChunkMap &chunkMap, glm::ivec3 pos)
{
    if (!chunkExists(chunkMap, pos))
    {
        return nullptr;
    }
    return &chunkMap.at(pos);
}
