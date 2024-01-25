#pragma once
#include "depedencies.h"
#include "dynamic_array.h"
#include "sekai_reader.h"
#include <string>
#include <map>

class TexturePacker{
    AtlasArray textureAtlas;
    SekaiReader sekaiReader;
    std::map<std::string, glm::vec2> textures;
public:

    void PackTextures();
    AtlasArray* GetTextureAtlas();
    std::map<std::string, glm::vec2>* GetTextures();
    u32 GetTextureAtlasSize();   

};