#pragma once

#include <unordered_map>
#include "world/chunkPos.h"
#include "PerlinNoise.hpp"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#define BLOCKS_PER_CHUNK CHUNK_SIZE *CHUNK_HEIGHT *CHUNK_SIZE

const siv::PerlinNoise::seed_type seed = 234516u;
const siv::PerlinNoise perlin{seed};

typedef std::unordered_map<ChunkPos, std::vector<char>, ChunkPosHash, ChunkPosEqual> ChunkDataMap;
