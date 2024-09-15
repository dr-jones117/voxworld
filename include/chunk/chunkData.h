#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "vec3.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 48
#define BLOCKS_PER_CHUNK CHUNK_SIZE *CHUNK_HEIGHT *CHUNK_SIZE

typedef std::unordered_map<glm::ivec2, std::vector<char>, Vec2Hash, Vec2Equal> ChunkDataMap;

bool chunkDataExists(ChunkDataMap &chunks, glm::ivec2 chunkPos);

void generateChunkDataFromPos(ChunkDataMap &chunks, glm::ivec2 startPos);
void generateChunkData(ChunkDataMap &chunks, glm::ivec2 chunkPos);

std::vector<char> getChunkDataIfExists(ChunkDataMap &chunks, glm::ivec2 chunkPos);

void removeChunkDataFromMap(ChunkDataMap &chunks, glm::ivec2 chunkPos);
void removeUnneededChunks(ChunkDataMap &chunks, glm::ivec2 startChunkPos);