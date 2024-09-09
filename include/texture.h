#pragma once

class Texture
{
public:
    unsigned int Id;
    Texture(const char *filepath, int wrapType, int filterType);
    void bind();
};

void generateTextures();