#include "chunk/chunkData.h"
#include "chunk.h"
#include "block.h"

#include <iostream>

bool chunkDataExists(ChunkDataMap &chunks, glm::ivec2 chunkPos)
{
    if (chunks.find(chunkPos) == chunks.end())
        return false;
    return true;
}

void generateChunkData(ChunkDataMap &chunks, glm::ivec2 chunkPos)
{
    std::cout << "pos: " << chunkPos.x << "," << chunkPos.y << std::endl;
    std::vector<char> data;
    data.resize(BLOCKS_PER_CHUNK);

    for (int i = 0; i < CHUNK_SIZE; i++) // X-axis
    {
        for (int k = 0; k < CHUNK_SIZE; k++) // Z-axis
        {
            double freq = 0.005;
            double noise = perlin.octave2D_01(freq * (chunkPos.x * CHUNK_SIZE + i), freq * (chunkPos.y * CHUNK_SIZE + k), 8);
            int blocksInHeight = 0;
            int terrainHeight = (int)(noise * CHUNK_HEIGHT);

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

    chunks[chunkPos] = data;
}

std::vector<char> getChunkDataIfExists(ChunkDataMap &chunks, glm::ivec2 chunkPos)
{
    if (chunkDataExists(chunks, chunkPos))
    {
        return chunks[chunkPos];
    }
    return {};
}

void removeChunkDataFromMap(ChunkDataMap &chunks, glm::ivec2 chunkPos)
{
    if (chunkDataExists(chunks, chunkPos))
    {
        chunks.erase(chunkPos);
    }
}

void removeUnneededChunks(ChunkDataMap &chunks, glm::ivec2 startChunkPos)
{
    std::vector<glm::ivec2> chunkPosToRemove;
    for (const auto &pair : chunks)
    {
        glm::ivec2 pos = pair.first;
        glm::vec2 vector = pos - startChunkPos;
        if ((int)glm::length(vector) > (render_distance + 4))
        {
            chunkPosToRemove.push_back(pos);
        }
    }

    for (const auto &pos : chunkPosToRemove)
    {
        removeChunkDataFromMap(chunks, pos);
    }
}

void generateChunkDataFromPos(ChunkDataMap &chunks, glm::ivec2 startPos)
{
    int x = startPos.x;
    int z = startPos.y;

    glm::ivec2 currPos = startPos;
    if (!chunkDataExists(chunks, currPos))
    {
        generateChunkData(chunks, currPos);
    }

    for (int i = 1; i <= render_distance + 1; i++)
    {
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec2(x - i + j, z + i);
            if (!chunkDataExists(chunks, currPos))
            {
                generateChunkData(chunks, currPos);
            }
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec2(x + i, z + i - j);
            if (!chunkDataExists(chunks, currPos))
            {
                generateChunkData(chunks, currPos);
            }
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec2(x + i - j, z - i);
            if (!chunkDataExists(chunks, currPos))
            {
                generateChunkData(chunks, currPos);
            }
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            currPos = glm::ivec2(x - i, z - i + j);
            if (!chunkDataExists(chunks, currPos))
            {
                generateChunkData(chunks, currPos);
            }
        }
    }

    removeUnneededChunks(chunks, startPos);
}