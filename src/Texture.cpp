#include "texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(){ }

void Texture::InitializeTextureFromFile(std::string fileName){
    std::string filePath = "resources/textures/" + fileName;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  


    // load and generate the textures
    i32 width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    u8* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}


void Texture::InitializeTextureFromAtlas(AtlasArray* textureAtlas){
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureAtlas->GetSize(), textureAtlas->GetSize(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureAtlas->GetAtlasArray());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    textureAtlas->FreeArray();
}

void Texture::InitializeNoiseTexture(NoiseTexture* noiseTexture){
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, noiseTexture->GetWidth(), noiseTexture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, noiseTexture->GetNoiseTexture());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture(){
    glDeleteTextures(1, &textureID);
}

void Texture::ActivateTexture(i32 textureUnit){
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID); 
}

