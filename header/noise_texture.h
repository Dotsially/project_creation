#pragma once
#include "depedencies.h"
#include "stdlib.h"



class NoiseTexture{
private:    
    u8* noiseTexture;
    u32 width;
    u32 height;
public:
    void InitiliazeNoiseTexture(u32 width, u32 height);
    void DestroyNoiseTexture();
    void InsertNoiseValue(u8 noise1, u8 noise2, u8 noise3, u8 noise4, i32 x, i32 y);

    u8* GetNoiseTexture();
    u32 GetWidth();
    u32 GetHeight();
};