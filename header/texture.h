#pragma once
#include "depedencies.h"
#include "glad/glad.h"
#include <string>


class Texture{
    u32 textureID;
    
public:
    Texture(std::string fileName);
    ~Texture();

    void ActivateTexture();
};