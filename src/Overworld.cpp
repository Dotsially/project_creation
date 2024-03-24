#include "overworld.h"
#include <iostream> 

Overworld::Overworld(i32 width, i32 height){
    mask.InitializeMask(width,height);
    mask.CreateSphereMask(glm::vec2{width/2.0-1.0,height/2.0-1.0}, height/2.0 - 20.0f);

    noiseTexture.InitiliazeNoiseTexture(width, height);

    
    worldTiles = (TileData*)calloc(width*height, sizeof(TileData));
    this->width = width;
    this->height = height;
}

void Overworld::GenerateOverworld(){
    fnl_state noise = fnlCreateState();
    noise.seed = SDL_GetTicks64();
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.frequency = 0.01f;
    noise.fractal_type = FNL_FRACTAL_FBM;
    noise.octaves = 4;
    noise.lacunarity = 2;
    noise.gain = 0.6;
    noise.weighted_strength = 0.20;

    fnl_state tempNoise = fnlCreateState();
    tempNoise.seed = SDL_GetTicks64() + 1;
    tempNoise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    tempNoise.frequency = 0.02f;
    tempNoise.fractal_type = FNL_FRACTAL_FBM;
    tempNoise.octaves = 3;
    tempNoise.lacunarity = 1.5;
    tempNoise.gain = 0.2;
    tempNoise.weighted_strength = 0.1;

    fnl_state humidityNoise = fnlCreateState();
    humidityNoise.seed = SDL_GetTicks64() + 2;
    humidityNoise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    humidityNoise.frequency = 0.02f;
    humidityNoise.fractal_type = FNL_FRACTAL_FBM;
    humidityNoise.octaves = 3;
    humidityNoise.lacunarity = 2.0;
    humidityNoise.gain = 0.5;
    humidityNoise.weighted_strength = 0.2;

    for(i32 x = 0; x < width; x++){
        for(i32 y = 0; y < height; y++){
            f32 value = (fnlGetNoise2D(&noise, x , y) + 1.0) * 0.5f; 
            f32 tempValue = fnlGetNoise2D(&tempNoise, x , y);
            f32 humValue = (fnlGetNoise2D(&noise, x , y) + 1.0) * 0.5f;
            u8 noiseValue = glm::max(value * 255 - mask.GetMaskData()[y + x*height] * 255, 0.0f);
            u8 tileValue = value * 10;
            noiseTexture.InsertNoiseValue(noiseValue,0,0,255,x, y);
            
            f32 distance = abs((glm::abs(y - (height/2.0))/(height/2.0) * 10.0) -10.0);
            distance += tempValue;
            distance = glm::clamp(distance, 0.0f, 10.0f);
            u8 tileType;
            if(distance <= 3.5){
                tileType = 0;
            }
            else if(distance <= 8.5){
                tileType = 1;
            }
            else{
                tileType = 2;
            }

            tileValue = glm::max(tileValue - mask.GetMaskData()[y + x*height] * 10.0f,1.0f);
            worldTiles[y + x*height].tileHeight = tileValue;
            if(tileValue < 5){
                worldTiles[y + x*height].tileType = 3;
            }else{
                worldTiles[y + x*height].tileType = tileType;
            }

        }
    }  


}

Overworld::~Overworld(){
    mask.DestroyMask();
    noiseTexture.DestroyNoiseTexture();
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

NoiseTexture* Overworld::GetNoiseTexture(){
    return &noiseTexture;
}