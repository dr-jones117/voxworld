#pragma once

#include <deque>
#include <mutex>

#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "block.h"
#include "world/mesh.h"
#include "threading.h"

class World
{
public:
    World() : threadPool(3), dataThreadPool(2)
    {
        focusMesh.setDepthTest(false);
        intialDataGenerated = false;
    }

    void init();

    void generateNewChunks(ChunkPos chunkPos);
    void render();
    BLOCK getBlockData(glm::ivec3 blockPos);
    void removeBlock(glm::ivec3 blockPos);
    void updateFocusBlock(glm::ivec3 &pos, char &face);
    bool intialDataGenerated;

private:
    ThreadPool threadPool;
    ThreadPool dataThreadPool;

    std::mutex data_mtx;
    ChunkDataMap chunkDataMap;

    std::mutex mesh_mtx;
    ChunkMeshMap chunkMeshMap;

    std::mutex mesh_queue_mtx;
    std::deque<ChunkPos> chunksToMeshQueue;

    std::mutex data_queue_mtx;
    std::deque<ChunkPos> chunkDataQueue;

    Mesh focusMesh;

    bool posIsInQueue(std::deque<ChunkPos> &queue, ChunkPos &pos);

    // chunk data
    void addChunksToDataQueue(ChunkPos &chunkPos);
    void generateNextData();

    void generateChunkDataFromPos(ChunkPos pos, bool initial);
    void generateChunkData(ChunkPos pos);
    std::vector<char> &getChunkDataIfExists(ChunkPos pos);
    bool chunkDataExists(ChunkPos chunkPos);

    void removeChunkDataFromMap(ChunkPos pos);
    void removeUnneededChunkData(ChunkPos pos);

    // chunk mesh
    void initializeOpaqueChunk(ChunkMesh &chunkMesh);
    void initializeTransparentChunk(ChunkMesh &chunkMesh);
    void bindChunkOpaque(ChunkMesh &chunkMesh);
    void bindChunkTransparent(ChunkMesh &chunkMesh);
    void unbindChunk(ChunkMesh &chunkMesh);
    void addChunksToMeshQueue(ChunkPos pos);

    void renderChunkMeshes();
    void generateNextMesh();

    bool chunkMeshExists(ChunkPos pos);
    void removeChunkFromMap(ChunkPos pos);
    void removeUnneededChunkMeshes(ChunkPos pos);
    ChunkMesh *getChunkFromMap(ChunkPos pos);
};