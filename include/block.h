#pragma once

#include <glm/glm.hpp>

#include <vector>

#define ATLAS_SIZE 1024.f

enum BLOCK
{
    AIR_BLOCK = 0,
    GRASS_BLOCK = 1,
    DIRT_BLOCK = 2,
    STONE_BLOCK = 3
};

typedef struct
{
    float u1, v1, u2, v2;
} UVcoords;

typedef struct
{
    char cover;
    glm::vec3 blockPos;
    std::vector<Vertex> &chunkVertices;
    std::vector<unsigned int> &chunkIndices;
    unsigned int &indiceOffset;
} BlockRenderInfo;

static UVcoords getUVcoords(int row, int col)
{
    float tileWidthU = 16.0f / ATLAS_SIZE;  // = 0.015625
    float tileHeightV = 16.0f / ATLAS_SIZE; // = 0.015625

    float startU = col * tileWidthU;  // = 3 * 0.015625 = 0.046875
    float startV = row * tileHeightV; // = 2 * 0.015625 = 0.03125

    // Texture coordinates for the quad
    float u1 = startU;               // 0.046875
    float v1 = startV;               // 0.03125
    float u2 = startU + tileWidthU;  // 0.046875 + 0.015625 = 0.0625
    float v2 = startV + tileHeightV; // 0.03125 + 0.015625 = 0.046875

    return {u1, v1, u2, v2};
}

static std::unordered_map<BLOCK, std::vector<UVcoords>> blockTextureCoords = {
    {BLOCK::AIR_BLOCK, {}},
    {BLOCK::GRASS_BLOCK, {getUVcoords(0, 0), getUVcoords(0, 1), getUVcoords(0, 2)}},
    {BLOCK::DIRT_BLOCK, {getUVcoords(0, 0), getUVcoords(0, 0), getUVcoords(0, 0)}},
    {BLOCK::STONE_BLOCK, {getUVcoords(0, 3), getUVcoords(0, 3), getUVcoords(0, 3)}},
};

void renderRegularBlock(BlockRenderInfo &renderInfo, std::vector<UVcoords> &coords)
{
    UVcoords bottomTexCoords = coords[0];
    UVcoords sideTexCoords = coords[1];
    UVcoords topTexCoords = coords[2];

    // North face
    if ((renderInfo.cover & 1) == 1)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(sideTexCoords.u1, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(sideTexCoords.u2, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(sideTexCoords.u2, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(sideTexCoords.u1, sideTexCoords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    if ((renderInfo.cover & 2) == 2)
    {
        // South face
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(sideTexCoords.u1, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(sideTexCoords.u2, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(sideTexCoords.u2, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(sideTexCoords.u1, sideTexCoords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    if ((renderInfo.cover & 4) == 4)
    {
        // West face
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(sideTexCoords.u1, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(sideTexCoords.u2, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(sideTexCoords.u2, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(sideTexCoords.u1, sideTexCoords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    if ((renderInfo.cover & 8) == 8)
    {
        // East face
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(sideTexCoords.u1, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(sideTexCoords.u2, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(sideTexCoords.u2, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(sideTexCoords.u1, sideTexCoords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    if ((renderInfo.cover & 16) == 16)
    {
        // Bottom face
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(bottomTexCoords.u1, bottomTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(bottomTexCoords.u2, bottomTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(bottomTexCoords.u2, bottomTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(bottomTexCoords.u1, bottomTexCoords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    if ((renderInfo.cover & 32) == 32)
    {
        // Top face
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(topTexCoords.u1, topTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(topTexCoords.u2, topTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(topTexCoords.u2, topTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(topTexCoords.u1, topTexCoords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
}

void renderAirBlock(BlockRenderInfo &renderInfo, std::vector<UVcoords> &coords)
{
    return;
}

static std::unordered_map<BLOCK, void (*)(BlockRenderInfo &renderInfo, std::vector<UVcoords> &)> blockRenderFunctions = {
    {BLOCK::AIR_BLOCK, renderAirBlock},
    {BLOCK::GRASS_BLOCK, renderRegularBlock},
    {BLOCK::DIRT_BLOCK, renderRegularBlock},
    {BLOCK::STONE_BLOCK, renderRegularBlock},
};
