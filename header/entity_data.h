#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"

typedef struct EntityData{ 
    glm::vec3 position; 
    glm::vec3 initialPosition;   
    glm::vec3 moveDirection;
    glm::vec3 forward;
    glm::vec3 right;
    f32 percentMoved;
    i8 moveDirectionValue = -1;
    i8 lookDirection;
    i8 flipped = 1;
    u8 isPlayer = 0;
    u8 isMoving = 0;
    u8 isFalling = 0;
    u8 shelterHeight = 32;
    u8 directionalTexture = 1;
    u8 modelType;
    u8 walkable;
    u8 consumable;
    u8 item;
    u8 pickup;
    u8 textureDirection = 0;
}EntityData;