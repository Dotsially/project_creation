#pragma once
#include "depedencies.h"
#include "tile_data.h"
#include "FastNoiseLite.h"

class Overworld{
private:
    TileData* worldTiles;
    i32 width = 0;
    i32 height = 0;
public:
    void GenerateOverworld(i32 width, i32 height);
    void DestroyOverworld();
    TileData* GetWorldTiles();
    i32 GetWidth();
    i32 GetHeight();
};