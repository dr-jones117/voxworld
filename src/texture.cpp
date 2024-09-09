#include <glad/glad.h>
#include <iostream>

#include "texture.h"
#include "glError.h"
#include "stb_image.h"
#include "voxwrld.h"

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

void generateTextures()
{
    textures[(int)(GRASS_BLOCK + BOTTOM_TEXTURE)] = new Texture("./res/textures/grassBottom.jpg", GL_REPEAT, GL_NEAREST);
    textures[(int)(GRASS_BLOCK + SIDE_TEXTURE)] = new Texture("./res/textures/grass.jpg", GL_REPEAT, GL_NEAREST);
    textures[(int)(GRASS_BLOCK + TOP_TEXTURE)] = new Texture("./res/textures/grassTop.jpg", GL_REPEAT, GL_NEAREST);
}