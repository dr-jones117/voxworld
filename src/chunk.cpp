#include <glad/glad.h>

#include "chunk.h"
#include "voxwrld.h"
#include "glError.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Chunk *generateChunk(glm::vec3 pos)
{
    Chunk *chunk = new Chunk();
    chunk->pos = pos;

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
                model = glm::translate(model, glm::vec3(chunk->pos.x + (float)i, chunk->pos.y + (float)j, chunk->pos.z + (float)k));

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
    chunks.push_back(generateChunk(glm::vec3(0.0f, 0.0f, 0.0f)));
    for (int i = 0; i < RENDER_DISTANCE; i++)
    {
        chunks.push_back(generateChunk(glm::vec3((float)i * CHUNK_SIZE, 0, (float)i * CHUNK_SIZE - CHUNK_SIZE)));
        chunks.push_back(generateChunk(glm::vec3((float)i * CHUNK_SIZE, 0, (float)i * CHUNK_SIZE + CHUNK_SIZE)));
        chunks.push_back(generateChunk(glm::vec3((float)i * CHUNK_SIZE - CHUNK_SIZE, (float)i * CHUNK_SIZE, 0)));
        chunks.push_back(generateChunk(glm::vec3((float)i * CHUNK_SIZE + CHUNK_SIZE, (float)i * CHUNK_SIZE, 0)));

        chunks.push_back(generateChunk(glm::vec3((float)i * CHUNK_SIZE - CHUNK_SIZE, 0, (float)i * CHUNK_SIZE - CHUNK_SIZE)));
        chunks.push_back(generateChunk(glm::vec3((float)i * CHUNK_SIZE + CHUNK_SIZE, 0, (float)i * CHUNK_SIZE + CHUNK_SIZE)));
        chunks.push_back(generateChunk(glm::vec3((float)i * CHUNK_SIZE - CHUNK_SIZE, (float)i * CHUNK_SIZE, 0 + CHUNK_SIZE)));
        chunks.push_back(generateChunk(glm::vec3((float)i * CHUNK_SIZE + CHUNK_SIZE, (float)i * CHUNK_SIZE, 0 - CHUNK_SIZE)));
    }
}
