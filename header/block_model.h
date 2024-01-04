#pragma once
#include "depedencies.h"
#include "sekai_reader.h"
#include "block_model_data.h"
#include <map>
#include <string>

class BlockModel{
private:    
    SekaiReader sekaiReader;
    std::map<std::string, BlockModelData> blockModels;
public:
    BlockModel();
    ~BlockModel();
    void CreateCuboidVertices(BlockModelData* model);
    BlockModelData* GetModels(std::string modelID);
    std::map<std::string, BlockModelData>* GetBlockModels();
};