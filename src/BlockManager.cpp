#include "block_manager.h"
#include <iostream>

BlockManager::BlockManager(std::map<std::string, glm::vec2>* textures){
    blocks[0] = BlockData{"air",0};
    blocks[255] = BlockData{
        "invalid_block", 
        255,
        "cube",
        glm::vec2(9,0),
        glm::vec2(9,0),
        glm::vec2(9,0),
        glm::vec2(9,0),
        glm::vec2(9,0),
        glm::vec2(9,0),
    };

    sekaiReader.ReadBlocks(&blockNames, &blocks, textures);
}

BlockManager::~BlockManager(){
}

std::map<u8, BlockData>* BlockManager::GetBlocks(){
    return &blocks;
}

u8 BlockManager::BlockNameToBlockId(std::string name){
    auto validName = blockNames.find(name);
    if(validName != blockNames.end()){
        return blockNames[name];
    }
    else{
        return 255;
    }
}