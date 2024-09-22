#pragma once

#include <deque>
#include <mutex>

#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "block.h"
#include "world/mesh.h"

class World
{
public:
    World()
    {
        focusMesh.setDepthTest(false);
    }

    void init();

    void generateNewChunks(ChunkPos chunkPos);
    void render();
    BLOCK getBlockData(glm::ivec3 blockPos);
    void removeBlock(glm::ivec3 blockPos);
    void updateFocusBlock(glm::ivec3 &pos, char &face);

private:
    std::mutex data_mtx;
    ChunkDataMap chunkDataMap;

    std::mutex mesh_mtx;
    ChunkMeshMap chunkMeshMap;

    std::deque<ChunkPos> chunksToMeshQueue;

    Mesh focusMesh;

    // chunk data
    void generateChunkDataFromPos(ChunkPos pos);
    void generateChunkData(ChunkPos pos);
    std::vector<char> &getChunkDataIfExists(ChunkPos pos);
    bool chunkDataExists(ChunkPos chunkPos);

    void removeChunkDataFromMap(ChunkPos pos);
    void removeUnneededChunkData(ChunkPos pos);

    // chunk mesh
    void initializeChunkGL(ChunkMesh &chunkMesh);
    void bindChunk(ChunkMesh &chunkMesh);
    void unbindChunk(ChunkMesh &chunkMesh);

    void addChunksToMeshQueue(ChunkPos pos);

    void renderChunkMeshes();
    void generateChunkMesh(ChunkPos pos);
    void generateNextMesh();

    bool chunkMeshExists(ChunkPos pos);
    void removeChunkFromMap(ChunkPos pos);
    void removeUnneededChunkMeshes(ChunkPos pos);
    ChunkMesh *getChunkFromMap(ChunkPos pos);
};