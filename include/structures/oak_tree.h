#pragma once

#include "block.h"

namespace Structure
{
    typedef struct
    {
        static const unsigned int x = 5; // width
        static const unsigned int y = 5; // height (vertical layers)
        static const unsigned int z = 5; // depth
        BLOCK blocks[x][y][z] = {
            {
                // Layer 0 (Bottom of the tree, base)
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, OAK_WOOD, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
            },
            {
                // Layer 1 (Trunk)
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, OAK_WOOD, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
            },
            {
                // Layer 2 (More trunk)
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, OAK_WOOD, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK, AIR_BLOCK},
            },
            {
                // Layer 3 (Leaves)
                {AIR_BLOCK, AIR_BLOCK, OAK_LEAVES, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, OAK_LEAVES, OAK_LEAVES, OAK_LEAVES, AIR_BLOCK},
                {OAK_LEAVES, OAK_LEAVES, OAK_WOOD, OAK_LEAVES, OAK_LEAVES},
                {AIR_BLOCK, OAK_LEAVES, OAK_LEAVES, OAK_LEAVES, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, OAK_LEAVES, AIR_BLOCK, AIR_BLOCK},
            },
            {
                // Layer 4 (Top of the tree)
                {AIR_BLOCK, AIR_BLOCK, OAK_LEAVES, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, OAK_LEAVES, AIR_BLOCK, AIR_BLOCK},
                {OAK_LEAVES, OAK_LEAVES, OAK_LEAVES, OAK_LEAVES, OAK_LEAVES},
                {AIR_BLOCK, AIR_BLOCK, OAK_LEAVES, AIR_BLOCK, AIR_BLOCK},
                {AIR_BLOCK, AIR_BLOCK, OAK_LEAVES, AIR_BLOCK, AIR_BLOCK},
            },
        };
    } OAK_TREE_ONE;
}
