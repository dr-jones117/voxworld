#include "block.h"

void renderRegularBlock(BlockRenderInfo &renderInfo)
{
    std::vector<UVcoords> textureCoords = blockTextureCoords[renderInfo.block];
    UVcoords bottomTexCoords = textureCoords[0];
    UVcoords sideTexCoords = textureCoords[1];
    UVcoords topTexCoords = textureCoords[2];

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

void renderAirBlock(BlockRenderInfo &renderInfo)
{
    return;
}
