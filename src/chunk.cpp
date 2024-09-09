#include <glad/glad.h>

#include "chunk.h"
#include "voxwrld.h"
#include "glError.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

int render_distance = 3;

Chunk *generateChunk(int x, int y, int z)
{
    Chunk *chunk = new Chunk();
    chunk->pos = glm::vec3((float)x, (float)y, (float)z);
    std::cout << "generating chunk: (" << x << ", " << y << ", " << z << ")" << std::endl;

    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_HEIGHT; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                chunk->data[i * j * k] = (BLOCK_TYPE)GRASS_BLOCK;
            }
        }
    }

    return chunk;
}

void renderChunk(Chunk *chunk, Shader *shader)
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_HEIGHT; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                Texture *botT = textures[chunk->data[i * j * k] + BOTTOM_TEXTURE];
                Texture *sideT = textures[chunk->data[i * j * k] + SIDE_TEXTURE];
                Texture *topT = textures[chunk->data[i * j * k] + TOP_TEXTURE];

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((chunk->pos.x * (float)CHUNK_SIZE) + (float)i,
                                                        (chunk->pos.y * (float)CHUNK_HEIGHT) + (float)j,
                                                        (chunk->pos.z * (float)CHUNK_SIZE) + (float)k));

                unsigned int modelLoc = glGetUniformLocation(shader->Id, "model");
                GLCall(glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)));

                sideT->bind();
                GLCall(glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void *)0));

                botT->bind();
                GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float))));

                topT->bind();
                GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float))));
            }
        }
    }
}

void generateChunks(glm::ivec3 currPos, std::vector<Chunk *> &chunks)
{
    int x = currPos.x;
    int y = currPos.y;
    int z = currPos.z;

    chunks.push_back(generateChunk(x, y, z));
    for (int i = 1; i <= render_distance; i++)
    {
        // start top left
        for (int j = 0; j < i * 2; j++)
        {
            chunks.push_back(generateChunk(x - i + j, y, z + i));
        }
        // start top right
        for (int j = 0; j < i * 2; j++)
        {
            chunks.push_back(generateChunk(x + i, y, z + i - j));
        }
        // start bottom right
        for (int j = 0; j < i * 2; j++)
        {
            chunks.push_back(generateChunk(x + i - j, y, z - i));
        }
        // start bottom left
        for (int j = 0; j < i * 2; j++)
        {
            chunks.push_back(generateChunk(x - i, y, z - i + j));
        }
    }
}
