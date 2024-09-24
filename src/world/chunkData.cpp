#include "world/chunkData.h"
#include "world/chunkMesh.h"
#include "world/world.h"
#include "block.h"

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
    std::srand(static_cast<unsigned>(std::time(0))); // Seed with current time
    int randomNumber = std::rand();                  // Generate a random number
    for (int i = 0; i < CHUNK_SIZE; i++)             // X-axis
    {
        for (int j = 0; j < CHUNK_HEIGHT; j++) // Y-axis
        {
            for (int k = 0; k < CHUNK_SIZE; k++) // Z-axis
            {
                double freq = 0.005;
                // double noise = perlin.octave3D_01(freq * (pos.x * CHUNK_SIZE + i), freq * j, freq * (pos.z * CHUNK_SIZE + k), 8);
                randomNumber = std::rand();
                // std::cout << "Random Number: " << randomNumber << std::endl;
                if (randomNumber < 15000)
                {
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
        for (int k = 0; k < CHUNK_SIZE; k++) // Z-axis
        {
            double freq = 0.005;
            double noise = perlin.octave2D_01(freq * (pos.x * CHUNK_SIZE + i), freq * (pos.z * CHUNK_SIZE + k), 8);
            int blocksInHeight = 0;
            int terrainHeight = (int)(noise * (CHUNK_HEIGHT - (CHUNK_HEIGHT / 2))) + (CHUNK_HEIGHT / 8);

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
                        data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::DIRT_BLOCK;
                    }

                    blocksInHeight++;
                }
                else if (j < terrainHeight)
                {
                    data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::GRASS_BLOCK;
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

    if (chunkDataExists(pos))
    {
        return chunkDataMap[pos];
    }
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

void World::generateChunkDataFromPos(ChunkPos pos)
{
    int x = pos.x;
    int z = pos.z;

    ChunkPos currPos = pos;
    if (!chunkDataExists(currPos))
    {
        generateChunkData(currPos);
    }

    for (int i = 1; i <= render_distance + 1; i++)
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