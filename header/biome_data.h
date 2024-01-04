#pragma once
#include "depedencies.h"
#include <string>

typedef struct BiomeData{
    std::string biomeName;
    std::string biomeType;
    u8 height;
    u8 elevation;
    std::string biomeLayers[3];
    u8 biomeLayersBlockID[3];
}BiomeData;