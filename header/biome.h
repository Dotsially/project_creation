#pragma once
#include "depedencies.h"
#include "sekai_reader.h"
#include "biome_data.h"
#include "block_manager.h"
#include <map>
#include <string>

class Biome{
private:
    SekaiReader sekaiReader;
    std::map<std::string, BiomeData> biomes;
public:
    Biome(BlockManager* blockManager);
    ~Biome();
    BiomeData GetBiome(std::string biome);
};