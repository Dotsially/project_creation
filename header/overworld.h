#pragma once
#include "depedencies.h"
#include "tile_data.h"
#include "SDL2/SDL.h"
#include "FastNoiseLite.h"
#include "noise_texture.h"
#include "mask.h"

class Overworld{
private:
    TileData* worldTiles;
    NoiseTexture noiseTexture;
    i32 width = 0;
    i32 height = 0;
    Mask mask;
public:
    Overworld(i32 width, i32 height);
    ~Overworld();
    void GenerateOverworld();
    TileData* GetWorldTiles();
    NoiseTexture* GetNoiseTexture();
    i32 GetWidth();
    i32 GetHeight();
};