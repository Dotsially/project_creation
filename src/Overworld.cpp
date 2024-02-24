#include "overworld.h"
#include <iostream> 

void Overworld::GenerateOverworld(i32 width, i32 height){
    worldTiles = (TileData*)calloc(width*height, sizeof(TileData));
    this->width = width;
    this->height = height;

    fnl_state noise = fnlCreateState();
    noise.frequency = 0.02f;

    for(i32 x = 0; x < this->width; x++){
        for(i32 y = 0; y < this->height; y++){
            f32 value = (fnlGetNoise2D(&noise, x , y) + 1) * 5; 
            worldTiles[y + x*this->width].tileHeight = value < 5 ? 0 : 1;
        }
    }     
}

void Overworld::DestroyOverworld(){
    free(worldTiles);
}

TileData* Overworld::GetWorldTiles(){
    return worldTiles;
}
i32 Overworld::GetWidth(){
    return width;
}
i32 Overworld::GetHeight(){
    return height;
}