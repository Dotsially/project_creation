#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"
#include <string>

typedef struct ItemData{
    std::string name;
    glm::vec2 texture;
    i8 consumable;
}ItemData;