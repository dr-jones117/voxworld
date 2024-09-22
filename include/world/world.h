#pragma once

#include <deque>
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <functional>
#include <atomic>

#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "block.h"
#include "world/mesh.h"

class ThreadPool
{
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void enqueueJob(std::function<void()> job);
    void stop();

private:
    std::vector<std::thread> workers;       // Vector to hold all threads
    std::queue<std::function<void()>> jobs; // Queue for pending jobs

    std::mutex queueMutex;             // Protect access to the job queue
    std::condition_variable condition; // Used to notify threads about jobs
    std::atomic<bool> stopFlag;        // Flag to indicate stopping

    void workerThread(); // Worker function for threads
};

class World
{
public:
    World() : pool(10)
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
    ChunkDataMap chunkDataMap;

    ThreadPool pool;
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