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
    glm::vec2 top;
    glm::vec2 bottom;
    glm::vec2 left;
    glm::vec2 right;
    glm::vec2 front;
    glm::vec2 back;
}BlockData;

