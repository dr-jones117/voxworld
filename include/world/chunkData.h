#pragma once

#include <unordered_map>
#include <deque>

#include "world/chunkPos.h"
#include "block.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#define BLOCKS_PER_CHUNK CHUNK_SIZE *CHUNK_HEIGHT *CHUNK_SIZE
#define WATER_LEVEL 58

typedef std::unordered_map<ChunkPos, std::vector<char>, ChunkPosHash, ChunkPosEqual> ChunkDataMap;
typedef std::unordered_map<ChunkPos, std::deque<BlockWithPos>, ChunkPosHash, ChunkPosEqual> StructQueue;
