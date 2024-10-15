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

void generateWater(std::vector<char> &data, ChunkPos pos)
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int k = 0; k < CHUNK_SIZE; k++)
        {
            for (int j = 0; j < CHUNK_HEIGHT; j++)
            {
                if (j > WATER_LEVEL)
                    continue;

                if (data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] == BLOCK::AIR_BLOCK)
                {
                    data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::WATER_BLOCK;
                }
            }
        }
    }
}

void generateStructures(std::vector<char> &data, ChunkPos pos)
{
    // Lambda to calculate index in the data vector
    auto index = [&](int x, int y, int z) -> int
    {
        return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    };

    // Generate region noise to determine blending weight between plains, hills, and mountains
    float regionFreq = 0.005;
    std::string biome;

    // Randomly decide to generate a tree
    float treeChance = 0.005; // 1% chance to generate a tree per column
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            double regionNoise = perlin.octave2D_01(regionFreq * (pos.x * CHUNK_SIZE + x), regionFreq * (pos.z * CHUNK_SIZE + z), 4);

            // Determine the biome based on regionNoise
            if (regionNoise < 0.4)
            {
                biome = "Plains";
            }
            else if (regionNoise >= 0.4 && regionNoise < 0.7)
            {
                biome = "Hills";
            }
            else
            {
                biome = "Mountains";
            }

            if (biome == "Hills")
            {
                if (rand() / (float)RAND_MAX < treeChance)
                {
                    // Find ground level to place the tree
                    for (int y = CHUNK_HEIGHT - 1; y >= 0; y--)
                    {
                        if (data[index(x, y, z)] == BLOCK::GRASS_BLOCK)
                        {
                            // Generate a simple tree (trunk + leaves)
                            int trunkHeight = 7; // Set trunk height

                            // Generate leaves on top of the trunk
                            int leavesStartY = y + trunkHeight - 2; // Leaves start a bit lower to allow for wood extension
                            int leavesTopHeight = 3;                // Height of the leaf "top" layer
                            int leavesRadius = 2;                   // Set the radius of the cubic leaf "canopy"
                            float leafChance = 0.65f;               // 70% chance to place a leaf block

                            // Generate a defined top layer of leaves
                            for (int lx = -1; lx <= 1; lx++)
                            {
                                for (int lz = -1; lz <= 1; lz++)
                                {
                                    int leafX = x + lx;
                                    int leafZ = z + lz;
                                    int leafY = leavesStartY + leavesTopHeight;

                                    // Ensure indices are within bounds
                                    if (leafY < CHUNK_HEIGHT && leafX >= 0 && leafX < CHUNK_SIZE && leafZ >= 0 && leafZ < CHUNK_SIZE)
                                    {
                                        data[index(leafX, leafY, leafZ)] = BLOCK::OAK_LEAVES;
                                    }
                                }
                            }

                            // Generate the cubic leaf canopy
                            for (int lx = -leavesRadius; lx <= leavesRadius; lx++)
                            {
                                for (int lz = -leavesRadius; lz <= leavesRadius; lz++)
                                {
                                    for (int ly = 0; ly <= 2; ly++) // Add 3 layers of leaves
                                    {
                                        // Add randomness to the leaf placement
                                        if ((lz < 2 && lx < 2) || (rand() / (float)RAND_MAX < leafChance)) // Random chance for natural look
                                        {
                                            int leafX = x + lx;
                                            int leafZ = z + lz;
                                            int leafY = leavesStartY + ly;

                                            // Ensure indices are within bounds
                                            if (leafY < CHUNK_HEIGHT && leafX >= 0 && leafX < CHUNK_SIZE && leafZ >= 0 && leafZ < CHUNK_SIZE)
                                            {
                                                data[index(leafX, leafY, leafZ)] = BLOCK::OAK_LEAVES;
                                            }
                                        }
                                    }
                                }
                            }

                            // Generate trunk (WOOD_BLOCK)
                            for (int t = 0; t < trunkHeight; t++)
                            {
                                if (y + t < CHUNK_HEIGHT) // Ensure trunk stays within bounds
                                {
                                    data[index(x, y + t, z)] = BLOCK::OAK_WOOD;
                                }
                            }

                            break; // Once a tree is placed, no need to continue in this column
                        }
                    }
                }
            }
        }
    }
}

void generateCaves(std::vector<char> &data, ChunkPos pos)
{
    double freq = 0.05;    // Reduced frequency for larger caves
    double density = 0.28; // Adjust density to make caves rarer

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
                    if (block == BLOCK::BEDROCK_BLOCK || block == BLOCK::SAND_BLOCK || block == BLOCK::WATER_BLOCK)
                        continue;
                    data[i + (j * CHUNK_SIZE) + (k * CHUNK_SIZE * CHUNK_HEIGHT)] = BLOCK::AIR_BLOCK;
                }
            }
        }
    }
}

void World::generateChunkData(ChunkPos pos)
{
    std::vector<char> data(BLOCKS_PER_CHUNK);

    // Lambda to calculate index in the data vector
    auto index = [&](int x, int y, int z) -> int
    {
        return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    };

    // Lambda to assign block type based on conditions
    auto assignBlock = [&](int x, int y, int z, bool rockyTops, bool snowyTops, bool sandyTops, int blocksInHeight)
    {
        if (y < 3)
        {
            data[index(x, y, z)] = BLOCK::BEDROCK_BLOCK;
        }
        else if (blocksInHeight >= 3)
        {
            data[index(x, y, z)] = BLOCK::STONE_BLOCK;
        }
        else if (rockyTops)
        {
            data[index(x, y, z)] = BLOCK::STONE_BLOCK;
        }
        else if (snowyTops)
        {
            data[index(x, y, z)] = BLOCK::SNOW_BLOCK;
        }
        else if (sandyTops)
        {
            data[index(x, y, z)] = BLOCK::SAND_BLOCK;
        }
        else
        {
            data[index(x, y, z)] = (blocksInHeight >= 1) ? BLOCK::DIRT_BLOCK : BLOCK::GRASS_BLOCK;
        }
    };

    // Lambda for linear interpolation (LERP)
    auto lerp = [](double a, double b, double t) -> double
    {
        return a + t * (b - a);
    };

    float regionFreq = 0.005;

    // Loop through X and Z axes
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            // Generate region noise to determine blending weight between plains, hills, and mountains
            double regionNoise = perlin.octave2D_01(regionFreq * (pos.x * CHUNK_SIZE + x), regionFreq * (pos.z * CHUNK_SIZE + z), 4);

            // Define frequencies and heights for different region types
            double plainsFreq = 0.005, hillsFreq = 0.007, mountainsFreq = 0.03;
            double plainsHeightScale = CHUNK_HEIGHT / 4, hillsHeightScale = CHUNK_HEIGHT / 2, mountainsHeightScale = CHUNK_HEIGHT - 60;

            // Determine blend weights based on regionNoise
            double plainsWeight = (regionNoise < 0.4) ? (1.0 - regionNoise / 0.4) : 0.0;                      // Strong in plains zone
            double hillsWeight = (regionNoise >= 0.4 && regionNoise < 0.7) ? (regionNoise - 0.4) / 0.3 : 0.0; // Middle zone (hills)
            double mountainsWeight = (regionNoise >= 0.7) ? (regionNoise - 0.7) / 0.5 : 0.0;                  // Strong in mountain zone

            // Blend frequency and height scales based on weights
            double blendedFreq = lerp(lerp(plainsFreq, hillsFreq, plainsWeight), mountainsFreq, mountainsWeight);
            double blendedHeightScale = lerp(lerp(plainsHeightScale, hillsHeightScale, plainsWeight), mountainsHeightScale, mountainsWeight);

            // Get terrain noise using blended frequency
            double noise = perlin.octave2D_01(blendedFreq * (pos.x * CHUNK_SIZE + x), blendedFreq * (pos.z * CHUNK_SIZE + z), 12);

            // Scale terrain height based on the blended height scale
            int terrainHeight = static_cast<int>(noise * blendedHeightScale) + (CHUNK_HEIGHT / 8);

            bool rockyTops = (terrainHeight > 110 && terrainHeight < 118);
            bool snowyTops = (terrainHeight >= 118);
            bool sandyTops = (terrainHeight < WATER_LEVEL + 3);

            int blocksInHeight = 0;

            // Loop through Y-axis (top-down)
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--)
            {
                if (y <= terrainHeight)
                {
                    // Fill blocks based on height and top conditions
                    assignBlock(x, y, z, rockyTops, snowyTops, sandyTops, blocksInHeight);
                    blocksInHeight++;
                }
                else
                {
                    // Empty space (AIR_BLOCK) above terrain height
                    data[index(x, y, z)] = BLOCK::AIR_BLOCK;
                }
            }
        }
    }

    generateWater(data, pos);
    generateCaves(data, pos);
    generateStructures(data, pos);

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
        if ((int)glm::length(vector) > (render_distance + 6))
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