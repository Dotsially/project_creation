#pragma once
#include "depedencies.h"
#include "entity_data.h"
#include "server/networking.h"
#include "world.h"
#include "camera.h"
#include <vector>
#include <map>


class EntityManager{
private:
    std::map<u32, EntityData> entities;
    u32 entityInControlID = 0;
    u32 entityCount = 0;

    void ProcessInput(EntityData* player, u32 playerID, ENetPeer* server, const u8* keystate, Camera* camera);
    void ProcessMovement(EntityData* entity, World* world);
    void Move(EntityData* entity);
    u8 CheckSheltered(EntityData* entity, World* world);
public:
    i32 SpawnEntity(glm::vec3 position);
    void SpawnPlayer(glm::vec2 position, World* world);
    i32 SpawnEntity(glm::vec3 position, u32 entityID);
    void SpawnPlayer(glm::vec2 position, u32 entityID, World* world);
    void DestroyEntity(u32 entityID);

    void Update(Camera* camera, World* world, ENetPeer* server);
    std::map<u32, EntityData>* GetEntities();
    EntityData* GetPlayer();
    EntityData* GetEntity(i32 entityID);
    glm::vec3 GetPlayerPosition();
    u8 GetPlayerShelterHeight();
};