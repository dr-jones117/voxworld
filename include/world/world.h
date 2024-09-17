#pragma once

#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "block.h"
#include "world/mesh.h"

class World
{
public:
    World()
    {
    }

    void init();

    void generateNewChunks(ChunkPos chunkPos);
    void render();
    BLOCK getBlockData(glm::ivec3 blockPos);
    void removeBlock(glm::ivec3 blockPos);
    void updateFocusBlock(glm::ivec3 &pos, char &face);

private:
    ChunkDataMap chunkDataMap;
    ChunkMeshMap chunkMeshMap;
    Mesh focusMesh;

    // chunk data
    void generateChunkDataFromPos(ChunkPos pos);
    void generateChunkData(ChunkPos pos);
    std::vector<char> &getChunkDataIfExists(ChunkPos pos);
    bool chunkDataExists(ChunkPos chunkPos);

    void removeChunkDataFromMap(ChunkPos pos);
    void removeUnneededChunkData(ChunkPos pos);

    // chunk mesh
    void bindChunk(ChunkMesh &chunkMesh);
    void unbindChunk(ChunkMesh &chunkMesh);

    void renderChunkMeshes();
    void generateChunkMeshes(ChunkPos pos);
    void generateChunkMesh(ChunkPos pos);

    bool chunkMeshExists(ChunkPos pos);
    void removeChunkFromMap(ChunkPos pos);
    void removeUnneededChunkMeshes(ChunkPos pos);
    ChunkMesh *getChunkFromMap(ChunkPos pos);
};