#include "block.h"

void renderLiquidBlock(BlockRenderInfo &renderInfo)
{
    std::vector<UVcoords> textureCoords = blockTextureCoords[renderInfo.block];
    UVcoords coords = textureCoords[0];

    // North face
    if ((renderInfo.cover & 1) == 1)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(coords.u1, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(coords.u2, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.9f, 0.0f), glm::vec2(coords.u2, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.9f, 0.0f), glm::vec2(coords.u1, coords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // South face
    if ((renderInfo.cover & 2) == 2)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.9f, 1.0f), glm::vec2(coords.u1, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.9f, 1.0f), glm::vec2(coords.u2, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(coords.u2, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(coords.u1, coords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // West face
    if ((renderInfo.cover & 4) == 4)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(coords.u1, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(coords.u2, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.9f, 0.0f), glm::vec2(coords.u2, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.9f, 1.0f), glm::vec2(coords.u1, coords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // East face
    if ((renderInfo.cover & 8) == 8)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.9f, 1.0f), glm::vec2(coords.u1, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.9f, 0.0f), glm::vec2(coords.u2, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(coords.u2, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(coords.u1, coords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // Bottom face
    if ((renderInfo.cover & 16) == 16)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(coords.u1, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(coords.u2, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(coords.u2, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(coords.u1, coords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // Top face
    if ((renderInfo.cover & 32) == 32)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.9f, 0.0f), glm::vec2(coords.u1, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.9f, 0.0f), glm::vec2(coords.u2, coords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.9f, 1.0f), glm::vec2(coords.u2, coords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.9f, 1.0f), glm::vec2(coords.u1, coords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
}

void renderRegularBlock(BlockRenderInfo &renderInfo)
{
    std::vector<UVcoords> textureCoords = blockTextureCoords[renderInfo.block];
    UVcoords bottomTexCoords = textureCoords[0];
    UVcoords sideTexCoords = textureCoords[1];
    UVcoords topTexCoords = textureCoords[2];

    // North face
    if ((renderInfo.cover & 1) == 1)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(sideTexCoords.u1, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(sideTexCoords.u2, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(sideTexCoords.u2, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(sideTexCoords.u1, sideTexCoords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // South face
    if ((renderInfo.cover & 2) == 2)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(sideTexCoords.u1, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(sideTexCoords.u2, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(sideTexCoords.u2, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(sideTexCoords.u1, sideTexCoords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // West face
    if ((renderInfo.cover & 4) == 4)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(sideTexCoords.u1, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(sideTexCoords.u2, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(sideTexCoords.u2, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(sideTexCoords.u1, sideTexCoords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // East face
    if ((renderInfo.cover & 8) == 8)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(sideTexCoords.u1, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(sideTexCoords.u2, sideTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(sideTexCoords.u2, sideTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(sideTexCoords.u1, sideTexCoords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // Bottom face
    if ((renderInfo.cover & 16) == 16)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(bottomTexCoords.u1, bottomTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(bottomTexCoords.u2, bottomTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(bottomTexCoords.u2, bottomTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(bottomTexCoords.u1, bottomTexCoords.v2)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
    // Top face
    if ((renderInfo.cover & 32) == 32)
    {
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(topTexCoords.u1, topTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(topTexCoords.u2, topTexCoords.v2)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(topTexCoords.u2, topTexCoords.v1)});
        renderInfo.chunkVertices.push_back({renderInfo.blockPos + glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(topTexCoords.u1, topTexCoords.v1)});

        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 1);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 2);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 3);
        renderInfo.chunkIndices.push_back(renderInfo.indiceOffset + 0);

        renderInfo.indiceOffset += 4;
    }
}

void renderAirBlock(BlockRenderInfo &renderInfo)
{
    return;
}
