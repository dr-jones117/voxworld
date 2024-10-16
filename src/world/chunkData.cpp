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
inline int positiveMod(int value, int mod)
{
    return (value % mod + mod) % mod;
}

void World::addStructureBlockToWorld(ChunkPos &pos, BlockWithPos &blockWithPos, std::vector<char> &data)
{
    auto index = [&](int x, int y, int z) -> int
    {
        return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
    };

    // Check if block is within the current chunk's boundaries
    if (blockWithPos.x >= 0 && blockWithPos.x < CHUNK_SIZE &&
        blockWithPos.y >= 0 && blockWithPos.y < CHUNK_HEIGHT &&
        blockWithPos.z >= 0 && blockWithPos.z < CHUNK_SIZE)
    {
        // Block is inside the current chunk, update the data array
        data[index(blockWithPos.x, blockWithPos.y, blockWithPos.z)] = blockWithPos.block;
    }
    else
    {
        // Block is outside the chunk, calculate the chunk position and add to structQueue
        ChunkPos newChunkPos;
        newChunkPos.x = pos.x + (blockWithPos.x < 0 ? (blockWithPos.x - CHUNK_SIZE + 1) / CHUNK_SIZE : blockWithPos.x / CHUNK_SIZE);
        newChunkPos.z = pos.z + (blockWithPos.z < 0 ? (blockWithPos.z - CHUNK_SIZE + 1) / CHUNK_SIZE : blockWithPos.z / CHUNK_SIZE);

        unsigned int localX = positiveMod(blockWithPos.x, CHUNK_SIZE);
        unsigned int localZ = positiveMod(blockWithPos.z, CHUNK_SIZE);

        BlockWithPos adjustedBlock = {localX, blockWithPos.y, localZ, blockWithPos.block};

        if (chunkDataMap.find(newChunkPos) != chunkDataMap.end())
        {
            auto &data = chunkDataMap[newChunkPos];
            data[index(adjustedBlock.x, adjustedBlock.y, adjustedBlock.z)] = adjustedBlock.block;
        }
        else
        {
            structQueue[newChunkPos].push_back(adjustedBlock);
        }
    }
}

void World::generateStructures(std::vector<char> &data, ChunkPos pos)
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
                    // std::cout << "MAKING TREEE!" << std::endl;
                    //  Find ground level to place the tree
                    for (int y = CHUNK_HEIGHT - 1; y >= 0; y--)
                    {
                        if (data[index(x, y, z)] == BLOCK::GRASS_BLOCK)
                        {
                            int trunkHeight = 8;
                            int leavesStartY = y + trunkHeight - 2;
                            int leavesTopHeight = 3;
                            int leavesRadius = 2;
                            float leafChance = 0.65f;

                            // Generate top layer of leaves
                            for (int lx = -1; lx <= 1; lx++)
                            {
                                for (int lz = -1; lz <= 1; lz++)
                                {
                                    int leafX = x + lx;
                                    int leafZ = z + lz;
                                    int leafY = leavesStartY + leavesTopHeight;

                                    BlockWithPos block = {leafX, leafY, leafZ, BLOCK::OAK_LEAVES};
                                    addStructureBlockToWorld(pos, block, data);
                                }
                            }

                            // Generate cubic leaf canopy
                            for (int lx = -leavesRadius; lx <= leavesRadius; lx++)
                            {
                                for (int lz = -leavesRadius; lz <= leavesRadius; lz++)
                                {
                                    for (int ly = 0; ly <= 2; ly++)
                                    {
                                        if ((lz < 2 && lx < 2) || (rand() / (float)RAND_MAX < leafChance))
                                        {
                                            int leafX = x + lx;
                                            int leafZ = z + lz;
                                            int leafY = leavesStartY + ly;

                                            BlockWithPos block = {leafX, leafY, leafZ, BLOCK::OAK_LEAVES};
                                            addStructureBlockToWorld(pos, block, data);
                                        }
                                    }
                                }
                            }

                            for (int t = 0; t < trunkHeight; t++)
                            {
                                int trunkY = y + t;
                                BlockWithPos block = {x, trunkY, z, BLOCK::OAK_WOOD};
                                addStructureBlockToWorld(pos, block, data);
                            }

                            break;
                        }
                    }
                }
            }
        }
    }

    // Pop from the queue to get the block data for the current chunk
    auto &queue = structQueue[pos];
    while (!queue.empty())
    {
        BlockWithPos block = queue.front(); // Get the block at the front of the queue
        queue.pop_front();                  // Remove it from the queue

        // Calculate index and place the block in the data vector
        if (block.y < CHUNK_HEIGHT) // Ensure the block is within bounds
        {
            data[index(block.x, block.y, block.z)] = block.block; // Update the data vector with the block
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
    // std::cout << "generating chunk: (" << pos.x << ", " << pos.z << ")" << std::endl;
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

    std::lock_guard<std::mutex> struct_lock(struct_mtx);
    std::lock_guard<std::mutex> lock(data_mtx);
    generateStructures(data, pos);
    chunkDataMap[pos] = data;
}

std::vector<char> &World::getChunkDataIfExists(ChunkPos pos)
{
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
        range = 2;
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