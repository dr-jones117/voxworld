#pragma once

#include <unordered_map>
#include "world/chunkPos.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#define BLOCKS_PER_CHUNK CHUNK_SIZE *CHUNK_HEIGHT *CHUNK_SIZE

typedef std::unordered_map<ChunkPos, std::vector<char>, ChunkPosHash, ChunkPosEqual> ChunkDataMap;
