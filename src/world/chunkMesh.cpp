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

int render_distance = 5;

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
    chunkMesh.isInitialized = false;

    unsigned int indiceOffset = 0;

    auto chunkData = getChunkDataIfExists({pos.x, pos.z});
    auto northChunkData = getChunkDataIfExists({pos.x, pos.z - 1});
    auto southChunkData = getChunkDataIfExists({pos.x, pos.z + 1});
    auto westChunkData = getChunkDataIfExists({pos.x - 1, pos.z});
    auto eastChunkData = getChunkDataIfExists({pos.x + 1, pos.z});

    assert(chunkData.size() > 0 && northChunkData.size() > 0 && southChunkData.size() > 0 && westChunkData.size() > 0 && eastChunkData.size() > 0);

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
                    if (northChunkData.size() > 0 && northChunkData[x + (y * CHUNK_SIZE) + ((CHUNK_SIZE - 1) * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
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
                    if (southChunkData.size() > 0 && southChunkData[x + (y * CHUNK_SIZE) + (0 * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
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
                    if (westChunkData.size() > 0 && westChunkData[(CHUNK_SIZE - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
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
                    if (eastChunkData.size() > 0 && eastChunkData[0 + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
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

    std::unique_lock<std::mutex> mesh_lock(mesh_mtx);
    chunkMeshMap[pos] = chunkMesh;
}

void World::generateNextMesh()
{

    std::unique_lock<std::mutex> queue_mtx(mesh_queue_mtx);

    if (chunksToMeshQueue.empty())
        return;
    ChunkPos pos = chunksToMeshQueue.front();

    if (!chunkDataExists({pos.x, pos.z}) || !chunkDataExists({pos.x, pos.z - 1}) || !chunkDataExists({pos.x, pos.z + 1}) || !chunkDataExists({pos.x - 1, pos.z}) || !chunkDataExists({pos.x + 1, pos.z}))
        return;

    chunksToMeshQueue.pop_front();

    queue_mtx.unlock();

    ChunkMesh chunkMesh;
    chunkMesh.pos = pos;
    chunkMesh.isInitialized = false;

    unsigned int indiceOffset = 0;

    auto chunkData = getChunkDataIfExists({pos.x, pos.z});
    auto northChunkData = getChunkDataIfExists({pos.x, pos.z - 1});
    auto southChunkData = getChunkDataIfExists({pos.x, pos.z + 1});
    auto westChunkData = getChunkDataIfExists({pos.x - 1, pos.z});
    auto eastChunkData = getChunkDataIfExists({pos.x + 1, pos.z});

    assert(chunkData.size() > 0 && northChunkData.size() > 0 && southChunkData.size() > 0 && westChunkData.size() > 0 && eastChunkData.size() > 0);

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
                    if (northChunkData.size() > 0 && northChunkData[x + (y * CHUNK_SIZE) + ((CHUNK_SIZE - 1) * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
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
                    if (southChunkData.size() > 0 && southChunkData[x + (y * CHUNK_SIZE) + (0 * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
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
                    if (westChunkData.size() > 0 && westChunkData[(CHUNK_SIZE - 1) + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
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
                    if (eastChunkData.size() > 0 && eastChunkData[0 + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
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

    std::unique_lock<std::mutex> mesh_lock(mesh_mtx);
    chunkMeshMap[pos] = chunkMesh;
}

void World::initializeChunkGL(ChunkMesh &chunkMesh)
{
    GLCall(glGenVertexArrays(1, &chunkMesh.VAO));
    GLCall(glGenBuffers(1, &chunkMesh.VBO));
    GLCall(glGenBuffers(1, &chunkMesh.EBO));

    bindChunk(chunkMesh);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(glm::vec3)));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * chunkMesh.vertices.size(), &chunkMesh.vertices.front(), GL_STATIC_DRAW));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * chunkMesh.indices.size(), &chunkMesh.indices.front(), GL_STATIC_DRAW));

    unbindChunk(chunkMesh);

    chunkMesh.isInitialized = true;
}

void World::renderChunkMeshes()
{
    std::lock_guard<std::mutex> lock(mesh_mtx);
    for (auto &pair : chunkMeshMap)
    {
        ChunkMesh &chunk = pair.second;
        if (chunk.indices.size() <= 0)
            return;
        if (!chunk.isInitialized)
            initializeChunkGL(chunk);

        bindChunk(chunk);

        GLCall(glDrawElements(GL_TRIANGLES, chunk.indices.size(), GL_UNSIGNED_INT, (void *)0));

        unbindChunk(chunk);
    }
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
        if ((int)glm::length(vector) > (render_distance + 1))
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
