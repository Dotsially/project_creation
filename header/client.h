#pragma once
#include "depedencies.h"
#include "server/networking.h"
#include "glm/glm.hpp"
#include "entity_manager.h"
#include "world.h"
#include "enet/enet.h"
#include <string>

class Client{
private:
    ENetAddress address = {};
    ENetHost* client = nullptr;
    ENetPeer* server = nullptr;
    u8 connected = 0;

    void SpawnEntity(EntityManager* entities, World* world, std::string pos, std::string id, i32 type);
public:
    void InitializeClient();
    void DestroyClient();
    void Update(World* world, EntityManager* entities);
    void Connect(std::string ip, i32 port);
    void SendMessageServer(std::string message);
    void ReceiveHandshake(World* world, EntityManager* entities);
    ENetPeer* GetServer();
};