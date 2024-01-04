#pragma once
#include "depedencies.h"
#include <vector>
#include <string>

typedef struct BlockModelData{
    std::string modelName;
    std::string modelType;
    f32 from[3];
    f32 to[3];
    std::vector<f32> vertices;
}BlockModelData;