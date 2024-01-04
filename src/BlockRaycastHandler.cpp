#include "block_raycast_handler.h"
#include <iostream>


void BlockRaycastHandler::Update(Camera* camera, World* world, const u8* keystate){
    if(keystate[SDL_SCANCODE_1]){
        currentBlock = {7,0,0,0};
    }
    else if(keystate[SDL_SCANCODE_2]){
        currentBlock = {6,0,0,0};
    }
    else if(keystate[SDL_SCANCODE_3]){
        currentBlock = {3,0,0,0};
    }
    else if(keystate[SDL_SCANCODE_4]){
        currentBlock = {4,0,0,0};
    }
    else if(keystate[SDL_SCANCODE_5]){
        currentBlock = {5,0,0,0};
    }

    i32 x,y;
    SDL_GetMouseState(&x,&y);
    
    //Get ray from mouse coordinates in 3d space
    glm::vec3 rayNds = glm::vec3((2.0f * x) / 1280.0f - 1.0f, 1.0f - (2.0f * y) / 720.0f, 1.0f);
    glm::vec4 rayClip = glm::vec4(rayNds.x, rayNds.y, -1.0, 1.0);
    glm::vec4 rayEye = glm::inverse(camera->GetProjectMatrix()) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
    glm::vec4 temp = (glm::inverse(camera->GetViewMatrix()) * rayEye);
    glm::vec3 rayWor = glm::vec3(temp.x, temp.y, temp.z);
    rayWor = glm::normalize(rayWor);

    //TODO MAKE THIS ITS OWN MOUSEHANDLER CLASS
    u8 LPressed = false;
    if(SDL_GetMouseState(&x,&y) & SDL_BUTTON_LMASK ){
        LPressed = true;
        if(isSolid){
            if(!blockBreakTimer.IsStarted()){   
                blockBreakTimer.Start(0.7);
                world->RemoveBlock(block.x,block.y,block.z);
            }
        }
    }
    else{
        LPressed = false;
    }

    if(!LPressed){
        blockBreakTimer.Stop();
    }

    u8 RPressed = false;
    if(SDL_GetMouseState(&x,&y) & SDL_BUTTON_RMASK ){
        RPressed = true;
        if(isSolid){
            if(!blockPlaceTimer.IsStarted()){
                blockPlaceTimer.Start(0.7);    
                world->AddBlock(block.x + blockNormal.x, block.y + blockNormal.y, block.z + blockNormal.z, currentBlock);
            }
        }
    }
    else{
        RPressed = false;
    }

    if(!RPressed){
        blockPlaceTimer.Stop();
    }

    blockBreakTimer.Count();
    blockPlaceTimer.Count();
    Raycast(camera, rayWor, world);
}


void BlockRaycastHandler::Raycast(Camera* camera, glm::vec3 rayDirection, World* world){
    isSolid = false;
    f32 deltaX, deltaY, deltaZ, maxX, maxY, maxZ;
    
    glm::vec3 pos1 = camera->GetPosition();
    glm::vec3 pos2 = camera->GetPosition() + rayDirection * 20.0f;



    i32 stepDir = -1;
    blockNormal = glm::vec3(0);

    i32 dx = glm::sign(pos2.x - pos1.x);
    if(dx != 0){
        deltaX = glm::min(dx / (pos2.x - pos1.x), 10000000.0f);
    }else{
        deltaX = 10000000.0f;
    }
    if(dx > 0){
        maxX = deltaX * (1.0f - glm::fract(pos1.x));
    }else{
        maxX = deltaX * glm::fract(pos1.x);
    }
    block.x = (i32)pos1.x;

    i32 dy = glm::sign(pos2.y - pos1.y);
    if(dy != 0){
        deltaY = glm::min(dy / (pos2.y - pos1.y), 10000000.0f);
    }else{
        deltaY = 10000000.0f;
    }
    if(dy > 0){
        maxY = deltaY * (1.0f - glm::fract(pos1.y));
    }else{
        maxY = deltaY * glm::fract(pos1.y);
    }
    block.y = (i32)pos1.y;

    i32 dz = glm::sign(pos2.z - pos1.z);
    if(dz != 0){
        deltaZ = glm::min(dz / (pos2.z - pos1.z), 10000000.0f);
    }else{
        deltaZ = 10000000.0f;
    }
    if(dz > 0){
        maxZ = deltaZ * (1.0f - glm::fract(pos1.z));
    }else{
        maxZ = deltaZ * glm::fract(pos1.z);
    } 
    block.z = (i32)pos1.z;

    while (true)
    {
        if (maxX < maxY) {
            if (maxX < maxZ) {
                block.x += dx;
                maxX += deltaX;
                stepDir = 0;
            } else {
                block.z += dz;
                maxZ += deltaZ;
                stepDir = 2;
            }
        } 
        else {
            if (maxY < maxZ) {
                block.y += dy;
                maxY += deltaY;
                stepDir = 1;
            } else {
                block.z += dz;
                maxZ += deltaZ;
                stepDir = 2;
            }
        }
        if (maxX > 1 && maxY > 1 && maxZ > 1){
            isSolid = false;
            break;
        }

        if(world->IsWithinWorld(block.x, block.y, block.z)){        
            if(world->ContainsBlock(block.x, block.y, block.z)){
                isSolid = true;
                
                if(stepDir == 0){
                    blockNormal.x = -dx;
                }
                else if(stepDir == 1){
                    blockNormal.y = -dy;
                }
                else{
                    blockNormal.z = -dz;
                } 
            
                break;
            }
            else{
                isSolid = false;
            }
        }
        else{
            isSolid = false;
        }
    }
}


glm::ivec3 BlockRaycastHandler::GetBlock(){
    return block;
}

bool BlockRaycastHandler::IsSolid(){
    return isSolid;
}