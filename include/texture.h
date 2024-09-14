#pragma once

class Texture
{
public:
    unsigned int Id;
    Texture(const char *filepath, int wrapType, int filterType);
    void bind();
};

typedef struct
{
    float u1, v1, u2, v2;
} UVcoords;

UVcoords getTextureCoordsFromAtlas(int row, int col);