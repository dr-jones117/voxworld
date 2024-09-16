#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "world/chunkMesh.h"
#include "world/world.h"

#include "glError.h"
#include "PerlinNoise.hpp"
#include "block.h"

int render_distance = 3;

void World::bindChunk(ChunkMesh &chunkMesh)
{
    GLCall(glBindVertexArray(chunkMesh.VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, chunkMesh.VBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.EBO));
}

void World::unbindChunk(ChunkMesh &chunkMesh)
{
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void World::generateChunkMesh(ChunkPos pos)
{
    ChunkMesh chunkMesh;
    chunkMesh.pos = pos;

    GLCall(glGenVertexArrays(1, &chunkMesh.VAO));
    GLCall(glGenBuffers(1, &chunkMesh.VBO));
    GLCall(glGenBuffers(1, &chunkMesh.EBO));

    bindChunk(chunkMesh);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(glm::vec3)));
    GLCall(glEnableVertexAttribArray(1));

    unbindChunk(chunkMesh);

    unsigned int indiceOffset = 0;

    auto chunkData = getChunkDataIfExists({pos.x, pos.z});
    auto northChunkData = getChunkDataIfExists({pos.x, pos.z - 1});
    auto southChunkData = getChunkDataIfExists({pos.x, pos.z + 1});
    auto westChunkData = getChunkDataIfExists({pos.x - 1, pos.z});
    auto eastChunkData = getChunkDataIfExists({pos.x + 1, pos.z});

    // if (northChunkData.size() == 0 || southChunkData.size() == 0 || westChunkData.size() == 0 || eastChunkData.size() == 0)
    // {
    //     return;
    // }

    for (int x = 0; x < CHUNK_SIZE; x++) // X-axis
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++) // Z-axis
        {
            for (int z = 0; z < CHUNK_SIZE; z++) // Y-axis
            {
                BLOCK block = (BLOCK)chunkData[x + y * CHUNK_SIZE + (z * CHUNK_SIZE * CHUNK_HEIGHT)];

                BlockRenderInfo renderInfo = {
                    block,
                    (char)0,
                    glm::vec3((pos.x * CHUNK_SIZE) + x, y, (pos.z * CHUNK_SIZE) + z),
                    chunkMesh.vertices,
                    chunkMesh.indices,
                    indiceOffset,
                };

                // North face
                int northIndex = x + (y * CHUNK_SIZE) + ((z - 1) * CHUNK_SIZE * CHUNK_HEIGHT);
                if (z <= 0)
                {
                    if (northChunkData[x + (y * CHUNK_SIZE) + ((CHUNK_SIZE - 1) * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
                    {
                        renderInfo.cover = renderInfo.cover | 1;
                    }
                }
                else if (chunkData[northIndex] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 1;
                }

                // South face
                int southIdx = x + (y * CHUNK_SIZE) + ((z + 1) * CHUNK_SIZE * CHUNK_HEIGHT);
                if (z >= CHUNK_SIZE - 1)
                {
                    if (southChunkData[x + (y * CHUNK_SIZE) + (0 * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
                    {
                        renderInfo.cover = renderInfo.cover | 2;
                    }
                }
                else if (chunkData[southIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 2;
                }

                // West face
                int westIdx = (x - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
                if (x <= 0)
                {
                    if (westChunkData[(CHUNK_SIZE - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
                    {
                        renderInfo.cover = renderInfo.cover | 4;
                    }
                }
                else if (chunkData[westIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 4;
                }

                // East face
                int eastIdx = (x + 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
                if (x >= CHUNK_SIZE - 1)
                {
                    if (eastChunkData[0 + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
                    {
                        renderInfo.cover = renderInfo.cover | 8;
                    }
                }
                else if (chunkData[eastIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 8;
                }

                // Bottom face
                int bottomIdx = x + ((y - 1) * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
                if (y <= 0 || chunkData[bottomIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 16;
                }
                // Top face
                int topIdx = x + ((y + 1) * CHUNK_SIZE) + ((z)*CHUNK_SIZE * CHUNK_HEIGHT);
                if (y >= CHUNK_HEIGHT - 1 || chunkData[topIdx] == BLOCK::AIR_BLOCK)
                {
                    renderInfo.cover = renderInfo.cover | 32;
                }

                blockRenderFunctions[block](renderInfo);
            }
        }
    }

    chunkMeshMap[pos] = chunkMesh;
}

void World::renderChunkMeshes()
{
    for (const auto &pair : chunkMeshMap)
    {
        ChunkMesh chunk = pair.second;
        bindChunk(chunk);

        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * chunk.vertices.size(), &chunk.vertices.front(), GL_STATIC_DRAW));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * chunk.indices.size(), &chunk.indices.front(), GL_STATIC_DRAW));
        GLCall(glDrawElements(GL_TRIANGLES, chunk.indices.size(), GL_UNSIGNED_INT, (void *)0));

        unbindChunk(chunk);
    }
}

bool World::chunkMeshExists(ChunkPos pos)
{
    if (chunkMeshMap.find(pos) == chunkMeshMap.end())
        return false;
    return true;
}

void World::removeChunkFromMap(ChunkPos pos)
{
    if (chunkMeshExists(pos))
    {
        chunkMeshMap.erase(pos);
    }
}

void World::removeUnneededChunkMeshes(ChunkPos pos)
{
    std::vector<ChunkPos> chunkPosToRemove;
    for (const auto &pair : chunkMeshMap)
    {
        ChunkPos chunkPos = pair.first;
        glm::vec3 vector = glm::vec3(chunkPos.x - pos.x, 0, chunkPos.z - pos.z);
        if ((int)glm::length(vector) > (render_distance + 3))
        {
            chunkPosToRemove.push_back(chunkPos);
        }
    }

    for (const auto &removePos : chunkPosToRemove)
    {
        removeChunkFromMap(removePos);
    }
}

void World::generateChunkMeshes(ChunkPos pos)
{
    int x = pos.x;
    int z = pos.z;

    ChunkPos currPos = pos;
    if (!chunkMeshExists(currPos))
    {
        generateChunkMesh(currPos);
    }

    for (int i = 1; i <= render_distance; i++)
    {
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x - i + j, z + i};
            if (!chunkMeshExists(currPos))
            {
                generateChunkMesh(currPos);
            }
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x + i, z + i - j};
            if (!chunkMeshExists(currPos))
            {
                generateChunkMesh(currPos);
            }
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x + i - j, z - i};
            if (!chunkMeshExists(currPos))
            {
                generateChunkMesh(currPos);
            }
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x - i, z - i + j};
            if (!chunkMeshExists(currPos))
            {
                generateChunkMesh(currPos);
            }
        }
    }

    removeUnneededChunkMeshes(pos);
}

ChunkMesh *World::getChunkFromMap(ChunkPos pos)
{
    if (!chunkMeshExists(pos))
    {
        return nullptr;
    }
    return &chunkMeshMap.at(pos);
}
