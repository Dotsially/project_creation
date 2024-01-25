#pragma once
#include "depedencies.h"
#include "glad/glad.h"
#include "dynamic_array.h"
#include <string>


class Texture{
    u32 textureID;
    
public:
    Texture();
    ~Texture();

    void InitializeTextureFromFile(std::string fileName);
    void InitializeTextureFromAtlas(AtlasArray* textureAtlas);

    void ActivateTexture();
};