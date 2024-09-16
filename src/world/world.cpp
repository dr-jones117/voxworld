#include <vector>
#include <iostream>

#include "world/world.h"
#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "world/chunkPos.h"
#include "block.h"

BLOCK World::getBlockData(glm::ivec3 blockPos)
{
    int chunkX = blockPos.x % CHUNK_SIZE;
    int chunkY = blockPos.y;
    int chunkZ = -(blockPos.z % CHUNK_SIZE);

    ChunkPos chunkPos = {blockPos.x / CHUNK_SIZE,
                         blockPos.z / CHUNK_SIZE};

    std::vector<char> data = getChunkDataIfExists(chunkPos);
    if (data.size() > 0)
    {
        BLOCK block = (BLOCK)data[chunkX + (chunkY * CHUNK_SIZE) + (chunkZ * CHUNK_SIZE * CHUNK_HEIGHT)];
        std::cout << "Found block: " << block << std::endl
                  << "at chunk: (" << chunkPos.x << ", " << chunkPos.z << ")" << std::endl
                  << "at pos: (" << chunkX << ", " << chunkY << ", " << chunkZ << ")" << std::endl;
        return block;
    }

    return BLOCK::AIR_BLOCK;
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