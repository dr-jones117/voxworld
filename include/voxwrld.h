#pragma once

#include "chunk.h"
#include "texture.h"

#include <unordered_map>

#define BLOCK_TYPE char
#define AIR_BLOCK 0x0000
#define GRASS_BLOCK 0x0001
#define WATER_BLOCK 0x0002

#define BOTTOM_TEXTURE 0x0000
#define SIDE_TEXTURE 0X0001
#define TOP_TEXTURE 0x0002

extern std::unordered_map<int, Texture *> textures;