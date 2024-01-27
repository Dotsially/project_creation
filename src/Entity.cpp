#include     "entity.h"
#include <iostream>

Entity::Entity(glm::vec2 position, World* world){
    this->position = glm::vec3(position.x, 
        world->GetTerrainHeight(position.x, position.y)+1,
        position.y);
    this->lookDirection = 0;
    this->forward = glm::vec3(-1,0,0);
    this->right = glm::vec3(0,0,-1);
}

Entity::~Entity(){


}

void Entity::Update(Camera* camera, World* world){
    if(flags.playerControlled == 1){
        const u8* keystate = SDL_GetKeyboardState(NULL);
        
        if(!isMoving){
            //this is more physics related move to different function...
            glm::vec3 wantedPosition = glm::vec3(position.x,
                position.y-1,
                position.z); 
            if(world->IsWithinWorld(wantedPosition)){ 
                if(!world->ContainsBlock(wantedPosition.x, 
                    wantedPosition.y,
                    wantedPosition.z)){
                        position.y--;
                }
            }
            ProcessInput(keystate, camera, world);
        }
        else if (inputDirection != glm::vec3(0,0,0)){
            Move();
        }
        else{
            isMoving = false;
        }
    }

    f32 rDot = glm::dot(right, camera->GetForward());
    f32 fDot = glm::dot(forward, camera->GetForward());

    if(fDot < -0.80f){
        flags.textureDirection = 0;
    }
    else if(fDot > 0.80f){
        flags.textureDirection = 4;
    }
    else{
        flags.flipped = rDot > 0.0;
        if(glm::abs(fDot) < 0.3){
            flags.textureDirection = 2;
        }
        else if(fDot < 0.0){
            flags.textureDirection = 1;
        }
        else{
            flags.textureDirection = 3;
        }
    }
}

glm::vec3 Entity::GetPosition(){
    return position;
}


void Entity::ProcessInput(const u8* keystate, Camera* camera, World* world)
{
    i8 moveDirection = -1;
    if(keystate[SDL_SCANCODE_W]){
        moveDirection = 0;
    }
    else if (keystate[SDL_SCANCODE_S]){
        moveDirection = 2;
    }
    else if (keystate[SDL_SCANCODE_A]){
        moveDirection = 3;
    }
    else if (keystate[SDL_SCANCODE_D]){
        moveDirection = 1;
    }

    if(moveDirection != -1){
        moveDirection = (moveDirection + camera->GetLookdirection()) %4;
    }

    
    inputDirection = glm::vec3(0,0,0);
    switch (moveDirection){
        case 0:
            inputDirection.z = 1;
            forward = glm::vec3(0,0,1);
            right = glm::vec3(-1,0,0);
            break;
        case 1:
            inputDirection.x = -1;
            forward = glm::vec3(-1,0,0);
            right = glm::vec3(0,0,-1);
            break;
        case 2:
            inputDirection.z = -1;
            forward = glm::vec3(0,0,-1);
            right = glm::vec3(1,0,0);
            break;
        case 3:
            inputDirection.x = 1;
            forward = glm::vec3(1,0,0);
            right = glm::vec3(0,0,1);
            break;
    }  

    if(inputDirection != glm::vec3(0,0,0)){
        //move to other function and refactor
        u8 validMovePosition = true;
        glm::vec3 wantedPosition = glm::vec3(position.x + inputDirection.x,
            position.y,
            position.z + inputDirection.z); 

        if(world->IsWithinWorld(wantedPosition)){ 
            if(world->ContainsBlock(wantedPosition.x, 
                wantedPosition.y+1,
                wantedPosition.z)){
                    validMovePosition = false;
            }

            if(world->ContainsBlock(wantedPosition.x, 
                wantedPosition.y,
                wantedPosition.z)){
                    if(world->ContainsBlock(wantedPosition.x, 
                        wantedPosition.y+2,
                        wantedPosition.z)){
                        validMovePosition = false;
                    }
                    else{
                        inputDirection.y = 1;
                    }
            }
        }else{
            validMovePosition = false;
        }

        if(validMovePosition){
            isMoving = true;
            initialPosition = position;
        }

    }
    
}

void Entity::Move(){
    percentMoved += .02f;
    if (percentMoved >= 1.0)
        {
            position.x = initialPosition.x + inputDirection.x;
            //add better climb later
            position.y = initialPosition.y + inputDirection.y;
            position.z = initialPosition.z + inputDirection.z;
            percentMoved = 0.0f;
            isMoving = false;
        }
        else
        {
            position.x = initialPosition.x + (inputDirection.x * percentMoved);
            position.z = initialPosition.z + (inputDirection.z * percentMoved);
        }
}
