#include "entity_manager.h"
#include <iostream>

i32 EntityManager::SpawnEntity(glm::vec3 position, u32 entityID){
    EntityData entity;
    entity.position = position;
    entity.directionalTexture = 1;
    entity.isPlayer = 0;
    entity.lookDirection = 0;
    entity.forward = glm::vec3(-1,0,0);
    entity.right = glm::vec3(0,0,-1);

    entities[entityID] = entity; 
    entityCount+=1; 

    return entityID;
}

void EntityManager::SpawnPlayer(glm::vec2 position, u32 entityID, World* world){
    EntityData player;
    player.position = glm::vec3(position.x, 
        world->GetTerrainHeight(position.x, position.y)+1,
        position.y);
    player.lookDirection = 0;
    player.forward = glm::vec3(-1,0,0);
    player.right = glm::vec3(0,0,-1);
    player.shelterHeight = CheckSheltered(&player, world);
    player.directionalTexture = 1;
    player.isPlayer = 1;

    entityInControlID = entityID;
    entities[entityID] = player;
    entityCount+=1;
}

i32 EntityManager::SpawnEntity(glm::vec3 position){
    return SpawnEntity(position, entityCount);
}

void EntityManager::SpawnPlayer(glm::vec2 position, World* world){
    SpawnPlayer(position, entityCount, world);
}

void EntityManager::DestroyEntity(u32 entityID){
    entities.erase(entityID);
}

void EntityManager::Update(Camera* camera, World* world, ENetPeer* server){
    const u8* keystate = SDL_GetKeyboardState(NULL);
    for(auto entity = entities.begin(); entity != entities.end(); entity++){
        if(!entity->second.isMoving){
            //this is more physics related move to different function...
            glm::vec3 wantedPosition = glm::vec3(entity->second.position.x,
                entity->second.position.y-1,
                entity->second.position.z); 
            if(world->IsWithinWorld(wantedPosition)){ 
                if(!world->ContainsBlock(wantedPosition.x, 
                    wantedPosition.y,
                    wantedPosition.z)){
                        entity->second.position.y--;
                    entity->second.isFalling = 1;
                }
                else{
                    entity->second.isFalling = 0;
                }
            }
            if(!entity->second.isFalling && entity->second.isPlayer){
                ProcessInput(&entity->second, entity->first, server, keystate, camera);
            }

            if(entity->second.moveDirectionValue != -1){
                ProcessMovement(&entity->second, world);
            }
        }
        else if (entity->second.moveDirection != glm::vec3(0,0,0)){
            Move(&entity->second);
        }
        else{
            entity->second.isMoving = 0;
            if(entity->second.isPlayer){
                u8 tempShelterHeight = entity->second.shelterHeight;
                entity->second.shelterHeight = CheckSheltered(&entity->second, world);
                if(entity->second.shelterHeight != tempShelterHeight){
                    
                }
            }
        }

        if(entity->second.directionalTexture){
            f32 rDot = glm::dot(entity->second.right, camera->GetForward());
            f32 fDot = glm::dot(entity->second.forward, camera->GetForward());

            if(fDot < -0.7f){
                entity->second.textureDirection = 0;
            }
            else if(fDot > 0.7f){
                entity->second.textureDirection = 4;
            }
            else{
                entity->second.flipped = rDot > 0.0 ? 1 : -1;
                if(glm::abs(fDot) < 0.3){
                    entity->second.textureDirection = 2;
                }
                else if(fDot < 0.0){
                    entity->second.textureDirection = 1;
                }
                else{
                    entity->second.textureDirection = 3;
                }
            }
        }
    }
}

glm::vec3 EntityManager::GetPlayerPosition(){
    return entities[entityInControlID].position;
}


void EntityManager::ProcessInput(EntityData* player, u32 playerID, ENetPeer* server, const u8* keystate, Camera* camera)
{
    if(keystate[SDL_SCANCODE_W]){
        player->moveDirectionValue = 0;
    }
    else if (keystate[SDL_SCANCODE_S]){
        player->moveDirectionValue = 2;
    }
    else if (keystate[SDL_SCANCODE_A]){
        player->moveDirectionValue = 3;
    }
    else if (keystate[SDL_SCANCODE_D]){
        player->moveDirectionValue = 1;
    }

    if(player->moveDirectionValue != -1){
        player->moveDirectionValue = (player->moveDirectionValue + camera->GetLookdirection()) %4;
        std::string data = "m";
            data+= "i" + std::to_string(playerID); 
            data+= "p" + std::to_string((int)player->position.x) + " " + std::to_string((int)player->position.y) + " " + std::to_string((int)player->position.z);
            data+= "d" + std::to_string(player->moveDirectionValue);
            SendMessage(data.c_str(), data.size()+1, server);
    }    
}

void EntityManager::ProcessMovement(EntityData* entity, World* world){
    entity->moveDirection = glm::vec3(0,0,0);
    switch (entity->moveDirectionValue){
        case 0:
            entity->moveDirection.z = 1;
            entity->forward = glm::vec3(0,0,1);
            entity->right = glm::vec3(-1,0,0);
            break;
        case 1:
            entity->moveDirection.x = -1;
            entity->forward = glm::vec3(-1,0,0);
            entity->right = glm::vec3(0,0,-1);
            break;
        case 2:
            entity->moveDirection.z = -1;
            entity->forward = glm::vec3(0,0,-1);
            entity->right = glm::vec3(1,0,0);
            break;
        case 3:
            entity->moveDirection.x = 1;
            entity->forward = glm::vec3(1,0,0);
            entity->right = glm::vec3(0,0,1);
            break;
    }  

    entity->moveDirectionValue = -1;

    if(entity->moveDirection != glm::vec3(0,0,0)){
        //move to other function and refactor
        u8 validMovePosition = true;
        glm::vec3 wantedPosition = glm::vec3(entity->position.x + entity->moveDirection.x,
            entity->position.y,
            entity->position.z + entity->moveDirection.z); 

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
                        entity->moveDirection.y = 1;
                    }
            }
        }else{
            validMovePosition = false;
        }

        if(validMovePosition){
            entity->isMoving = 1;
            entity->initialPosition = entity->position;
        }

    }


}
void EntityManager::Move(EntityData* entity){
    entity->percentMoved += .02f;
    if (entity->percentMoved >= 1.0)
        {
            entity->position.x = entity->initialPosition.x + entity->moveDirection.x;
            //add better climb later
            entity->position.y = entity->initialPosition.y + entity->moveDirection.y;
            entity->position.z = entity->initialPosition.z + entity->moveDirection.z;
            entity->percentMoved = 0.0f;
            entity->moveDirection = glm::vec3(0,0,0);
        }
        else
        {
            entity->position.x = entity->initialPosition.x + (entity->moveDirection.x * entity->percentMoved);
            entity->position.z = entity->initialPosition.z + (entity->moveDirection.z * entity->percentMoved);
        }
}


u8 EntityManager::CheckSheltered(EntityData* entity, World* world){
    u8 height = 32;  
    for(i32 i = 2; i < 15; i++){
        if(world->IsWithinWorld(entity->position.x, entity->position.y + i, entity->position.z)){
            if(world->ContainsBlock(entity->position.x, entity->position.y + i, entity->position.z)){
                height = entity->position.y + i;
                return height;
            }
        }else{
            break;
        }
    }

    return height;
}

u8 EntityManager::GetPlayerShelterHeight(){
    return entities[entityInControlID].shelterHeight;
}

EntityData* EntityManager::GetPlayer(){
    return &entities[entityInControlID];
}

EntityData* EntityManager::GetEntity(i32 entityID){
    return &entities[entityID];
}

std::map<u32, EntityData>* EntityManager::GetEntities(){
    return &entities;
}