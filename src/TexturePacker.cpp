#include "texture_packer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


void TexturePacker::PackTextures(){
    sekaiReader.ReadTextures(&textureAtlas, &textures);
}


AtlasArray* TexturePacker::GetTextureAtlas(){
    return &textureAtlas;
}
std::map<std::string, glm::vec2>* TexturePacker::GetTextures(){
    return &textures;
}

u32 TexturePacker::GetTextureAtlasSize(){
    return textureAtlas.GetSize();
}