#pragma once
#include "depedencies.h"
#include <string>
#include <map>
#include <vector>

typedef struct BiomeNoiseData{
    f32 frequency;
    f32 octaves;
    f32 lacuranity;
    f32 gain;
    f32 weightedStrength;
    u8 noiseType;
    u8 fractalType;
    u8 cellularReturnType;
    u8 divisor = 0;
}BiomeNoiseData;

typedef struct BiomeFogData{
    f32 fogColor[3];   
    f32 skyColor[3];
}BiomeFogData;

typedef struct BiomeData{
    std::string name;
    std::string type;
    u8 height;
    u8 elevation;
    std::vector<BiomeNoiseData> noise;
    std::map<std::string, BiomeFogData> colors;
    std::string layers[3];
    u8 biomeLayersBlockID[3];
}BiomeData;
