#pragma once
#include "depedencies.h"
#include <dirent.h>
#include <string>
#include <nlohmann/json.hpp>
#include "world_data.h"
#include "block_data.h"
#include "biome_data.h"
#include "block_model_data.h"


class SekaiReader{
private:
    std::vector<std::string> ReadDirectory(std::string path);
    BlockData ReadBlockFile(std::string path, u8 id);
    BiomeData ReadBiomeFile(std::string path);
    BlockModelData ReadBlockModelFile(std::string path);
public:
    void ReadBlocks(std::map<std::string, u8>* blockNames, std::map<u8, BlockData>* blocks);
    void ReadBiomes(std::map<std::string, BiomeData>* biomes);
    void ReadBlockModels(std::map<std::string, BlockModelData>* blockModels);
    void ReadWorld(WorldData* worldData);
};