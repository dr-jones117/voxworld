#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "world/world.h"
#include "block.h"

#include <random>

#include <iostream>

#include <cstdlib> // For std::rand and std::srand
#include <ctime>   // For std::time

bool World::chunkDataExists(ChunkPos pos)
{
    std::lock_guard<std::mutex> lock(data_mtx);
    if (chunkDataMap.find(pos) == chunkDataMap.end())
        return false;
    return true;
}

void generateCaves(std::vector<char> &data, ChunkPos pos)
{
    double freq = 0.03;   // Reduced frequency for larger caves
    double density = 0.2; // Adjust density to make caves rarer

    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_HEIGHT; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                if (j > 80)
                    continue;
                // Generate noise value
                double noise = perlin.octave3D_01(freq * (pos.x * CHUNK_SIZE + i), freq * j, freq * (pos.z * CHUNK_SIZE + k), 12);

                // Adjust the condition to create rarer but larger caves
                if (noise < (0.60 - density))
                {
                    BLOCK block = (BLOCK)data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)];
                    if (block == BLOCK::GRASS_BLOCK || block == BLOCK::DIRT_BLOCK)
                    {
                        // Create a random number generator and seed it
                        std::mt19937 generator((unsigned int)(freq * (pos.x * CHUNK_SIZE + i), freq * j, freq * (pos.z * CHUNK_SIZE + k)));

                        // Define a distribution (uniform distribution in this case)
                        std::uniform_int_distribution<int> distribution(1, 2);

                        // Generate a random number
                        int randomNumber = distribution(generator);
                        if (randomNumber == 1)
                            continue;
                    }
                    data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::AIR_BLOCK;
                }
            }
        }
    }
}

void World::generateChunkData(ChunkPos pos)
{
    std::vector<char> data;
    data.resize(BLOCKS_PER_CHUNK);

    for (int i = 0; i < CHUNK_SIZE; i++) // X-axis
    {
        bool rockyTops = false;
        for (int k = 0; k < CHUNK_SIZE; k++) // Z-axis
        {
            double freq = 0.003;
            double noise = perlin.octave2D_01(freq * (pos.x * CHUNK_SIZE + i), freq * (pos.z * CHUNK_SIZE + k), 8);
            int blocksInHeight = 0;
            int terrainHeight = (int)(noise * (CHUNK_HEIGHT - (CHUNK_HEIGHT / 2))) + (CHUNK_HEIGHT / 8);
            // std::cout << terrainHeight << std::endl;
            if (terrainHeight > 110)
            {
                rockyTops = true;
            }

            for (int j = CHUNK_HEIGHT - 1; j >= 0; j--) // Y-axis
            {
                if (blocksInHeight >= 1)
                {
                    if (blocksInHeight >= 3)
                    {
                        data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::STONE_BLOCK;
                    }
                    else
                    {
                        if (rockyTops)
                        {
                            data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::STONE_BLOCK;
                        }
                        else
                        {
                            data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::DIRT_BLOCK;
                        }
                    }

                    blocksInHeight++;
                }
                else if (j < terrainHeight)
                {
                    if (rockyTops)
                    {
                        data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::STONE_BLOCK;
                    }
                    else
                    {
                        data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::GRASS_BLOCK;
                    }
                    blocksInHeight++;
                }
                else
                {
                    data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::AIR_BLOCK;
                }
            }
        }
    }

    generateCaves(data, pos);

    std::lock_guard<std::mutex> lock(data_mtx);
    chunkDataMap[pos] = data;
}

std::vector<char> &World::getChunkDataIfExists(ChunkPos pos)
{
    std::lock_guard<std::mutex> lock(data_mtx);
    if (chunkDataMap.find(pos) != chunkDataMap.end())
    {
        return chunkDataMap[pos];
    }
    std::vector<char> emptyVec;
    return emptyVec;
}

void World::removeChunkDataFromMap(ChunkPos pos)
{
    chunkDataMap.erase(pos);
}

void World::removeUnneededChunkData(ChunkPos pos)
{
    std::lock_guard<std::mutex> lock(data_mtx);
    std::vector<ChunkPos> chunkPosToRemove;
    for (const auto &pair : chunkDataMap)
    {
        ChunkPos currPos = pair.first;
        glm::vec2 vector = glm::vec2(currPos.x - pos.x, currPos.z - pos.z);
        if ((int)glm::length(vector) > (render_distance + 3))
        {
            chunkPosToRemove.push_back(currPos);
        }
    }

    for (const auto &pos : chunkPosToRemove)
    {
        removeChunkDataFromMap(pos);
    }
}

void World::generateChunkDataFromPos(ChunkPos pos, bool initial = false)
{
    int x = pos.x;
    int z = pos.z;

    ChunkPos currPos = pos;
    if (!chunkDataExists(currPos))
    {
        generateChunkData(currPos);
    }

    int range = render_distance + 1;
    if (initial)
    {
        range = 1;
    }
    for (int i = 1; i <= range; i++)
    {
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x - i + j, z + i};
            if (!chunkDataExists(currPos))
            {
                generateChunkData(currPos);
            }
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x + i, z + i - j};
            if (!chunkDataExists(currPos))
            {
                generateChunkData(currPos);
            }
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x + i - j, z - i};
            if (!chunkDataExists(currPos))
            {
                generateChunkData(currPos);
            }
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x - i, z - i + j};
            if (!chunkDataExists(currPos))
            {
                generateChunkData(currPos);
            }
        }
    }
}

void World::addChunksToDataQueue(ChunkPos &pos)
{
    std::unique_lock<std::mutex> queue_mtx(data_queue_mtx);
    int x = pos.x;
    int z = pos.z;

    ChunkPos currPos = pos;
    if (!chunkDataExists(currPos) && !posIsInQueue(chunkDataQueue, currPos))
    {
        chunkDataQueue.push_back(currPos);
    }

    for (int i = 1; i < render_distance; i++)
    {
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x - i + j, z + i};
            if (!chunkDataExists(currPos) && !posIsInQueue(chunkDataQueue, currPos))
            {
                chunkDataQueue.push_back(currPos);
            }
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x + i, z + i - j};
            if (!chunkDataExists(currPos) && !posIsInQueue(chunkDataQueue, currPos))
            {
                chunkDataQueue.push_back(currPos);
            }
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x + i - j, z - i};
            if (!chunkDataExists(currPos) && !posIsInQueue(chunkDataQueue, currPos))
            {
                chunkDataQueue.push_back(currPos);
            }
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = {x - i, z - i + j};
            if (!chunkDataExists(currPos) && !posIsInQueue(chunkDataQueue, currPos))
            {
                chunkDataQueue.push_back(currPos);
            }
        }
    }
}

void World::generateNextData()
{
    std::lock_guard<std::mutex> data_queue_lock(data_queue_mtx);
    if (chunkDataQueue.empty())
        return;

    ChunkPos pos = chunkDataQueue.front();
    chunkDataQueue.pop_front();

    generateChunkData(pos);
}