#include <GL/glew.h>
#include <iostream>
#include "texture.h"
#include "glError.h"
#include "stb_image.h"

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