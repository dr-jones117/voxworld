#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "vec3.h"
#include "world.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#define BLOCKS_PER_CHUNK CHUNK_SIZE *CHUNK_HEIGHT *CHUNK_SIZE

typedef std::unordered_map<ChunkPos, std::vector<char>> ChunkDataMap;
