#include "block_model.h"


BlockModel::BlockModel(){
    sekaiReader.ReadBlockModels(&blockModels);
    
    for(auto& [key, model] : blockModels){
        if(model.modelType == "cuboid"){
            CreateCuboidVertices(&model);
        }
    }
    
}

BlockModel::~BlockModel(){
}

BlockModelData* BlockModel::GetModels(std::string modelID){
    auto validName = blockModels.find(modelID);
    if(validName != blockModels.end()){
        return &blockModels[modelID];
    }
    else{
        return nullptr;
    }

}

std::map<std::string, BlockModelData>* BlockModel::GetBlockModels(){
    return &blockModels;
}


void BlockModel::CreateCuboidVertices(BlockModelData* model){
    f32* from = model->from;
    f32* to = model->to;
    f32 step = 1.0/16.0;

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(from[1]*step);
    model->vertices.push_back(from[2]*step);

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(from[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(from[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(from[1]*step);
    model->vertices.push_back(from[2]*step);

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(from[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(from[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(from[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(from[2]*step);

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(from[1]*step);
    model->vertices.push_back(from[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(from[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(from[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(from[1]*step);
    model->vertices.push_back(from[2]*step);
    
    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(from[2]*step);

    model->vertices.push_back(from[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(to[2]*step);

    model->vertices.push_back(to[0]*step);
    model->vertices.push_back(to[1]*step);
    model->vertices.push_back(from[2]*step);
}