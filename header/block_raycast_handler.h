#pragma once 
#include "depedencies.h"
#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "camera.h"
#include "world.h"
#include "block_manager.h"
#include "block_data.h"
#include "timer.h"

class BlockRaycastHandler{
    BlockInstanceData currentBlock = {1,0,0,0};
    glm::ivec3 block;
    glm::ivec3 blockNormal;
    bool isSolid = false;
    void Raycast(Camera* camera, glm::vec3 rayDirection, World* chunk);
    Timer blockBreakTimer;
    Timer blockPlaceTimer;

public:
    void Update(Camera* camera, World* chunk, const u8* keystate);
    glm::ivec3 GetBlock();
    bool IsSolid();
};