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

int render_distance = 16;

void World::bindChunkOpaque(ChunkMesh &chunkMesh)
{
    GLCall(glBindVertexArray(chunkMesh.VAO_opaque));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, chunkMesh.VBO_opaque));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.EBO_opaque));
}

void World::bindChunkTransparent(ChunkMesh &chunkMesh)
{
    GLCall(glBindVertexArray(chunkMesh.VAO_transparent));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, chunkMesh.VBO_transparent));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.EBO_transparent));
}

void World::unbindChunk(ChunkMesh &chunkMesh)
{
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

typedef struct
{
    std::vector<char> chunkData;
    std::vector<char> northChunkData;
    std::vector<char> southChunkData;
    std::vector<char> westChunkData;
    std::vector<char> eastChunkData;
} ChunkData;

void updateLiquidRenderInfo(BLOCK block, int x, int y, int z, LiquidRenderInfo &renderInfo, ChunkData &chunkData)
{
    int northIndex = x + (y * CHUNK_SIZE) + ((z - 1) * CHUNK_SIZE * CHUNK_HEIGHT);
    int southIdx = x + (y * CHUNK_SIZE) + ((z + 1) * CHUNK_SIZE * CHUNK_HEIGHT);
    int westIdx = (x - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    int eastIdx = (x + 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    int bottomIdx = x + ((y - 1) * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    int topIdx = x + ((y + 1) * CHUNK_SIZE) + ((z)*CHUNK_SIZE * CHUNK_HEIGHT);

    // Bottom face
    if (y <= 0 || (chunkData.chunkData[bottomIdx] != block && chunkData.chunkData[bottomIdx] == BLOCK::AIR_BLOCK))
    {
        renderInfo.cover = renderInfo.cover | 16;
    }

    // Top face
    if (y >= CHUNK_HEIGHT - 1 || chunkData.chunkData[topIdx] != block)
    {
        renderInfo.cover = renderInfo.cover | 32;
    }
    else
    {
        renderInfo.liquidOnTop = true;
    }

    // North face
    if (z <= 0)
    {
        // check the north chunks data
        BLOCK northBlock = (BLOCK)chunkData.northChunkData[x + (y * CHUNK_SIZE) + ((CHUNK_SIZE - 1) * CHUNK_SIZE * CHUNK_HEIGHT)];
        if (northBlock == BLOCK::AIR_BLOCK)
        {
            renderInfo.cover = renderInfo.cover | 1;
        }
    }
    else if (chunkData.chunkData[northIndex] == BLOCK::AIR_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 1;
    }

    // South face
    if (z >= CHUNK_SIZE - 1)
    {
        BLOCK southBlock = (BLOCK)chunkData.southChunkData[x + (y * CHUNK_SIZE) + (0 * CHUNK_SIZE * CHUNK_HEIGHT)];
        if (southBlock == BLOCK::AIR_BLOCK)
        {
            renderInfo.cover = renderInfo.cover | 2;
        }
    }
    else if (chunkData.chunkData[southIdx] == BLOCK::AIR_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 2;
    }

    // West face
    if (x <= 0)
    {
        BLOCK westBlock = (BLOCK)chunkData.westChunkData[(CHUNK_SIZE - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)];
        if (westBlock == BLOCK::AIR_BLOCK)
        {
            renderInfo.cover = renderInfo.cover | 4;
        }
    }
    else if (chunkData.chunkData[westIdx] == BLOCK::AIR_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 4;
    }

    // East face
    if (x >= CHUNK_SIZE - 1)
    {
        BLOCK eastBlock = (BLOCK)chunkData.eastChunkData[0 + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)];
        if (eastBlock == BLOCK::AIR_BLOCK)
        {
            renderInfo.cover = renderInfo.cover | 8;
        }
    }
    else if (chunkData.chunkData[eastIdx] == BLOCK::AIR_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 8;
    }
}

void updateOpaqueRenderInfo(int x, int y, int z, BlockRenderInfo &renderInfo, ChunkData &chunkData)
{
    // North face
    int northIndex = x + (y * CHUNK_SIZE) + ((z - 1) * CHUNK_SIZE * CHUNK_HEIGHT);
    if (z <= 0)
    {
        // check the north chunks data
        BLOCK northBlock = (BLOCK)chunkData.northChunkData[x + (y * CHUNK_SIZE) + ((CHUNK_SIZE - 1) * CHUNK_SIZE * CHUNK_HEIGHT)];
        if (northBlock == BLOCK::AIR_BLOCK || northBlock == BLOCK::WATER_BLOCK)
        {
            renderInfo.cover = renderInfo.cover | 1;
        }
    }
    else if (chunkData.chunkData[northIndex] == BLOCK::AIR_BLOCK || chunkData.chunkData[northIndex] == BLOCK::WATER_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 1;
    }

    // South face
    int southIdx = x + (y * CHUNK_SIZE) + ((z + 1) * CHUNK_SIZE * CHUNK_HEIGHT);
    if (z >= CHUNK_SIZE - 1)
    {
        BLOCK southBlock = (BLOCK)chunkData.southChunkData[x + (y * CHUNK_SIZE) + (0 * CHUNK_SIZE * CHUNK_HEIGHT)];
        if (southBlock == BLOCK::AIR_BLOCK || southBlock == BLOCK::WATER_BLOCK)
        {
            renderInfo.cover = renderInfo.cover | 2;
        }
    }
    else if (chunkData.chunkData[southIdx] == BLOCK::AIR_BLOCK || chunkData.chunkData[southIdx] == BLOCK::WATER_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 2;
    }

    // West face
    int westIdx = (x - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    if (x <= 0)
    {
        BLOCK westBlock = (BLOCK)chunkData.westChunkData[(CHUNK_SIZE - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)];
        if (westBlock == BLOCK::AIR_BLOCK || westBlock == BLOCK::WATER_BLOCK)
        {
            renderInfo.cover = renderInfo.cover | 4;
        }
    }
    else if (chunkData.chunkData[westIdx] == BLOCK::AIR_BLOCK || chunkData.chunkData[westIdx] == BLOCK::WATER_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 4;
    }

    // East face
    int eastIdx = (x + 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    if (x >= CHUNK_SIZE - 1)
    {
        BLOCK eastBlock = (BLOCK)chunkData.eastChunkData[0 + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)];
        if (eastBlock == BLOCK::AIR_BLOCK || eastBlock == BLOCK::WATER_BLOCK)
        {
            renderInfo.cover = renderInfo.cover | 8;
        }
    }
    else if (chunkData.chunkData[eastIdx] == BLOCK::AIR_BLOCK || chunkData.chunkData[eastIdx] == BLOCK::WATER_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 8;
    }

    // Bottom face
    int bottomIdx = x + ((y - 1) * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    if (y <= 0 || chunkData.chunkData[bottomIdx] == BLOCK::AIR_BLOCK || chunkData.chunkData[bottomIdx] == BLOCK::WATER_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 16;
    }
    // Top face
    int topIdx = x + ((y + 1) * CHUNK_SIZE) + ((z)*CHUNK_SIZE * CHUNK_HEIGHT);
    if (y >= CHUNK_HEIGHT - 1 || chunkData.chunkData[topIdx] == BLOCK::AIR_BLOCK || chunkData.chunkData[topIdx] == BLOCK::WATER_BLOCK)
    {
        renderInfo.cover = renderInfo.cover | 32;
    }
}

void World::generateNextMesh()
{
    std::unique_lock<std::mutex> queue_mtx(mesh_queue_mtx);

    if (chunksToMeshQueue.empty())
        return;
    ChunkPos pos = chunksToMeshQueue.front();
    if (chunkGenerationTries > 10)
    {
        chunksToMeshQueue.pop_front();
        chunkGenerationTries = 0;
        return;
    }

    std::unique_lock<std::mutex> data_lock(data_mtx);
    if (!chunkDataExists({pos.x, pos.z}) || !chunkDataExists({pos.x, pos.z - 1}) || !chunkDataExists({pos.x, pos.z + 1}) || !chunkDataExists({pos.x - 1, pos.z}) || !chunkDataExists({pos.x + 1, pos.z}))
    {
        chunkGenerationTries++;
        return;
    }

    ChunkData chunkData = {
        chunkDataMap[{pos.x, pos.z}],
        chunkDataMap[{pos.x, pos.z - 1}],
        chunkDataMap[{pos.x, pos.z + 1}],
        chunkDataMap[{pos.x - 1, pos.z}],
        chunkDataMap[{pos.x + 1, pos.z}],
    };
    data_lock.unlock();

    chunksToMeshQueue.pop_front();
    queue_mtx.unlock();

    ChunkMesh chunkMesh;
    chunkMesh.pos = pos;
    chunkMesh.isInitialized = false;
    chunkMesh.transparentInitialized = false;

    unsigned int indiceOffset = 0;
    unsigned int transparentIndiceOffset = 0;

    assert(chunkData.chunkData.size() > 0 && chunkData.northChunkData.size() > 0 && chunkData.southChunkData.size() > 0 && chunkData.westChunkData.size() > 0 && chunkData.eastChunkData.size() > 0);

    for (int x = 0; x < CHUNK_SIZE; x++) // X-axis
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++) // Z-axis
        {
            for (int z = 0; z < CHUNK_SIZE; z++) // Y-axis
            {
                BLOCK block = (BLOCK)chunkData.chunkData[x + y * CHUNK_SIZE + (z * CHUNK_SIZE * CHUNK_HEIGHT)];

                if (block == BLOCK::WATER_BLOCK)
                {
                    LiquidRenderInfo liquidRenderInfo = {
                        block,
                        (char)0,
                        glm::vec3((pos.x * CHUNK_SIZE) + x, y, (pos.z * CHUNK_SIZE) + z),
                        chunkMesh.vertices_transparent,
                        chunkMesh.indices_transparent,
                        transparentIndiceOffset,
                        false,
                    };
                    updateLiquidRenderInfo(block, x, y, z, liquidRenderInfo, chunkData);
                    liquidRenderFunctions[block](liquidRenderInfo);
                }
                else
                {
                    BlockRenderInfo renderOpaqueInfo = {
                        block,
                        (char)0,
                        glm::vec3((pos.x * CHUNK_SIZE) + x, y, (pos.z * CHUNK_SIZE) + z),
                        chunkMesh.vertices_opaque,
                        chunkMesh.indices_opaque,
                        indiceOffset,
                    };
                    updateOpaqueRenderInfo(x, y, z, renderOpaqueInfo, chunkData);
                    blockRenderFunctions[block](renderOpaqueInfo);
                }
            }
        }
    }

    std::unique_lock<std::mutex> mesh_lock(mesh_mtx);
    chunkMeshMap[pos] = chunkMesh;
}

void World::initializeTransparentChunk(ChunkMesh &chunkMesh)
{
    GLCall(glGenVertexArrays(1, &chunkMesh.VAO_transparent));
    GLCall(glGenBuffers(1, &chunkMesh.VBO_transparent));
    GLCall(glGenBuffers(1, &chunkMesh.EBO_transparent));

    bindChunkTransparent(chunkMesh);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(glm::vec3)));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * chunkMesh.vertices_transparent.size(), &chunkMesh.vertices_transparent.front(), GL_STATIC_DRAW));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * chunkMesh.indices_transparent.size(), &chunkMesh.indices_transparent.front(), GL_STATIC_DRAW));

    unbindChunk(chunkMesh);

    chunkMesh.transparentInitialized = true;
}

void World::initializeOpaqueChunk(ChunkMesh &chunkMesh)
{
    GLCall(glGenVertexArrays(1, &chunkMesh.VAO_opaque));
    GLCall(glGenBuffers(1, &chunkMesh.VBO_opaque));
    GLCall(glGenBuffers(1, &chunkMesh.EBO_opaque));

    bindChunkOpaque(chunkMesh);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(glm::vec3)));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * chunkMesh.vertices_opaque.size(), &chunkMesh.vertices_opaque.front(), GL_STATIC_DRAW));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * chunkMesh.indices_opaque.size(), &chunkMesh.indices_opaque.front(), GL_STATIC_DRAW));

    unbindChunk(chunkMesh);

    chunkMesh.isInitialized = true;
}

void World::renderChunkMeshes()
{
    std::lock_guard<std::mutex> lock(mesh_mtx);

    // Render opaque chunks first (with depth writing and depth testing enabled)
    for (auto &pair : chunkMeshMap)
    {
        ChunkMesh &chunk = pair.second;

        if (chunk.indices_opaque.size() > 0)
        {
            if (!chunk.isInitialized)
                initializeOpaqueChunk(chunk);

            bindChunkOpaque(chunk);
            GLCall(glDrawElements(GL_TRIANGLES, chunk.indices_opaque.size(), GL_UNSIGNED_INT, (void *)0));
            unbindChunk(chunk);
        }
    }

    // Enable blending for transparency
    glDepthMask(GL_FALSE); // Disable depth writing for transparent blocks, but leave depth testing on
    glDisable(GL_CULL_FACE);
    //   Render transparent chunks next
    for (auto &pair : chunkMeshMap)
    {
        ChunkMesh &chunk = pair.second;

        if (chunk.indices_transparent.size() > 0)
        {
            if (!chunk.transparentInitialized)
                initializeTransparentChunk(chunk);

            bindChunkTransparent(chunk);
            GLCall(glDrawElements(GL_TRIANGLES, chunk.indices_transparent.size(), GL_UNSIGNED_INT, (void *)0));
            unbindChunk(chunk);
        }
    }

    // Re-enable depth writing and disable blending
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}

bool World::chunkMeshExists(ChunkPos pos)
{
    std::unique_lock<std::mutex> mesh_lock(mesh_mtx);
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
        if ((int)glm::length(vector) > (render_distance + 4))
        {
            chunkPosToRemove.push_back(chunkPos);
        }
    }

    for (const auto &removePos : chunkPosToRemove)
    {
        removeChunkFromMap(removePos);
    }
}

bool World::posIsInQueue(std::deque<ChunkPos> &queue, ChunkPos &pos)
{
    auto it = std::find(queue.begin(), queue.end(), pos);
    if (it != queue.end())
    {
        return true;
    }
    return false;
}

void World::addChunksToMeshQueue(ChunkPos pos)
{
    std::unique_lock<std::mutex> queue_mtx(mesh_queue_mtx);
    int x = pos.x;
    int z = pos.z;

    ChunkPos currPos = pos;
    if (!chunkMeshExists(currPos) && !posIsInQueue(chunksToMeshQueue, currPos))
    {
        chunksToMeshQueue.push_back(currPos);
    }

    for (int i = 1; i < render_distance; i++)
    {
        if (i == render_distance - 1)
        {
            int test = 10;
        }
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x - i + j, z + i};
            if (!chunkMeshExists(currPos) && !posIsInQueue(chunksToMeshQueue, currPos))
            {
                chunksToMeshQueue.push_back(currPos);
            }
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x + i, z + i - j};
            if (!chunkMeshExists(currPos) && !posIsInQueue(chunksToMeshQueue, currPos))
            {
                chunksToMeshQueue.push_back(currPos);
            }
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x + i - j, z - i};
            if (!chunkMeshExists(currPos) && !posIsInQueue(chunksToMeshQueue, currPos))
            {
                chunksToMeshQueue.push_back(currPos);
            }
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x - i, z - i + j};
            if (!chunkMeshExists(currPos) && !posIsInQueue(chunksToMeshQueue, currPos))
            {
                chunksToMeshQueue.push_back(currPos);
            }
        }
    }
}

ChunkMesh *World::getChunkFromMap(ChunkPos pos)
{
    if (!chunkMeshExists(pos))
    {
        return nullptr;
    }
    return &chunkMeshMap.at(pos);
}
