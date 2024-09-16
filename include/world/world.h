#pragma once

#include "world/chunkMesh.h"
#include "world/chunkData.h"

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
        renderChunkMeshes(chunkMeshMap);
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
    void removeUnneededChunks(ChunkPos pos);

    // chunk mesh
    void bindChunk(ChunkMesh &chunkMesh);
    void unbindChunk(ChunkMesh &chunkMesh);

    void renderChunkMeshes();
    void generateChunkMeshes(ChunkPos pos);
    void generateChunkMesh(ChunkPos pos);

    void renderChunkMeshes();

    bool chunkMeshExists(ChunkPos pos);
    void removeChunkFromMap(ChunkPos pos);
    void removeUnneededChunks(ChunkPos pos);
    ChunkMesh &getChunkFromMap(ChunkPos pos);
};