#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"
#include "block_model_data.h"
#include <string>


typedef struct BlockInstanceData{
    u8 blockID;
    u8 visibility =  0;
    u8 growthStage;
    u8 lookDirection;
} BlockInstanceData;

typedef struct BlockData{
    std::string blockNameID;
    u8 blockID;
    std::string blockModelID;
    glm::ivec2 top;
    glm::ivec2 bottom;
    glm::ivec2 left;
    glm::ivec2 right;
    glm::ivec2 front;
    glm::ivec2 back;
}BlockData;

