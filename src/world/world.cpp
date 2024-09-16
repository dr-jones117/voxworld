#include <vector>
#include <iostream>

#include "world/world.h"
#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "world/chunkPos.h"
#include "block.h"

BLOCK World::getBlockData(glm::ivec3 blockPos)
{
    int chunk_x = blockPos.x / CHUNK_SIZE;
    int chunk_z = blockPos.z / CHUNK_SIZE;

    int block_x = blockPos.x % CHUNK_SIZE;
    int block_y = blockPos.y;
    int block_z = blockPos.z % CHUNK_SIZE;

    if (block_x < 0)
    {
        block_x += CHUNK_SIZE;
        chunk_x -= 1;
    }

    if (block_y < 0)
    {
        block_y += CHUNK_SIZE;
    }

    if (block_z < 0)
    {
        block_z += CHUNK_SIZE;
        chunk_z -= 1;
    }

    ChunkPos chunkPos = {chunk_x,
                         chunk_z};

    std::vector<char> data = getChunkDataIfExists(chunkPos);
    if (data.size() > 0)
    {
        BLOCK block = (BLOCK)data[block_x + (block_y * CHUNK_SIZE) + (block_z * CHUNK_SIZE * CHUNK_HEIGHT)];
        std::cout << "Found block: " << block << std::endl
                  << "at chunk: (" << chunkPos.x << ", " << chunkPos.z << ")" << std::endl
                  << "at pos: (" << block_x << ", " << block_y << ", " << block_z << ")" << std::endl;
        return block;
    }

    return BLOCK::AIR_BLOCK;
}

void World::removeBlock(glm::ivec3 blockPos)
{
    int chunk_x = blockPos.x / CHUNK_SIZE;
    int chunk_z = blockPos.z / CHUNK_SIZE;

    int block_x = blockPos.x % CHUNK_SIZE;
    int block_y = blockPos.y;
    int block_z = blockPos.z % CHUNK_SIZE;

    if (block_x < 0)
    {
        block_x += CHUNK_SIZE;
        chunk_x -= 1;
    }

    if (block_y < 0)
    {
        block_y += CHUNK_SIZE;
    }

    if (block_z < 0)
    {
        block_z += CHUNK_SIZE;
        chunk_z -= 1;
    }

    ChunkPos chunkPos = {chunk_x,
                         chunk_z};

    std::vector<char> &data = getChunkDataIfExists(chunkPos);
    if (data.size() > 0)
    {
        data[block_x + (block_y * CHUNK_SIZE) + (block_z * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::AIR_BLOCK;
        // std::cout << "old data: " << (BLOCK)data[block_x + (block_y * CHUNK_SIZE) + (block_z * CHUNK_SIZE * CHUNK_HEIGHT)] << std::endl;
        // std::cout << "data: ";
        auto testData = chunkDataMap[chunkPos];
        BLOCK testBlock = (BLOCK)testData[block_x + (block_y * CHUNK_SIZE) + (block_z * CHUNK_SIZE * CHUNK_HEIGHT)];
        // std::cout << testBlock;
        generateChunkMesh(chunkPos);
    }
}

void World::generateNewChunks(ChunkPos chunkPos)
{
    generateChunkDataFromPos(chunkPos);
    generateChunkMeshes(chunkPos);
}

void World::render()
{
    renderChunkMeshes();
}