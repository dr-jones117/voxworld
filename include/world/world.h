#pragma once

#include "world/chunkData.h"
#include "world/chunkMesh.h"

class World
{
public:
    World()
    {
    }

    void generateNewChunks(ChunkPos chunkPos)
    {
        generateChunkDataFromPos(chunkPos);
        generateChunkMeshes(chunkPos);
    }

    void render()
    {
        renderChunkMeshes();
    }

private:
    ChunkDataMap chunkDataMap;
    ChunkMeshMap chunkMeshMap;

    // chunk data
    void generateChunkDataFromPos(ChunkPos pos);
    void generateChunkData(ChunkPos pos);
    std::vector<char> getChunkDataIfExists(ChunkPos pos);
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