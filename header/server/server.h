#pragma once
#include "depedencies.h"
#include "server/networking.h"
#include "server/player.h"
#include "enet/enet.h"
#include "entity_manager.h"
#include <map>

class Server{
private:
    ENetAddress address = {};
    ENetHost* server = nullptr;
    u8 running = 0;

    std::vector<Player> clients;

    EntityManager entities;


    void SendHandshake(Player* client, std::vector<Player> clients);
    void SendSpawn(Player* client, std::vector<Player> clients);
    void SendMovement(ENetPeer* client, std::vector<Player> clients, std::string data);
    i32 SendDisconnect(ENetPeer* client, std::vector<Player> clients);

public:    
    void InitializeServer();
    void DestroyServer();
    void Run();
    void Stop();
};