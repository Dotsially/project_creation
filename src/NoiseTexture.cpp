#include "noise_texture.h"


void NoiseTexture::InitiliazeNoiseTexture(u32 width, u32 height){
    this->width = width;
    this->height = height;
    noiseTexture = (u8*)calloc(width * height * 4, sizeof(u8));
}


void NoiseTexture::DestroyNoiseTexture(){
    free(noiseTexture);
}


void NoiseTexture::InsertNoiseValue(u8 noise1, u8 noise2, u8 noise3, u8 noise4, i32 x, i32 y){
        noiseTexture[(x*4+(y*4)*width)] = noise1;
        noiseTexture[(x*4+(y*4)*width)+1] = noise2;
        noiseTexture[(x*4+(y*4)*width)+2] = noise3;
        noiseTexture[(x*4+(y*4)*width)+3] = noise4;
}


u8* NoiseTexture::GetNoiseTexture(){
    return noiseTexture;
}

u32 NoiseTexture::GetWidth(){
    return width;
}

u32 NoiseTexture::GetHeight(){
    return height;
}