#include <glad/glad.h>
#include <iostream>

#include "texture.h"
#include "glError.h"
#include "stb_image.h"

#define ATLAS_SIZE 1024.f

Texture::Texture(const char *filepath, int wrapType, int filterType)
{
    GLCall(glGenTextures(1, &Id));
    GLCall(glBindTexture(GL_TEXTURE_2D, Id));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType));

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        std::cout << "Failed to load texture:" << Id << std::endl;
    }
    stbi_image_free(data);
}

void Texture::bind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, Id));
}

UVcoords getTextureCoordsFromAtlas(int row, int col)
{
    float tileWidthU = 16.0f / ATLAS_SIZE;
    float tileHeightV = 16.0f / ATLAS_SIZE;

    float startU = col * tileWidthU;
    float startV = row * tileHeightV;

    float u1 = startU;
    float v1 = startV;
    float u2 = startU + tileWidthU;
    float v2 = startV + tileHeightV;

    return {u1, v1, u2, v2};
}