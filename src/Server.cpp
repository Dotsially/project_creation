#include "server/server.h"
#include <iostream>

void Server::InitializeServer(){
    address.host = ENET_HOST_ANY;
    address.port = 2001;

    server = enet_host_create(&address, 32, 2, 0, 0);
                
    if(server == NULL){
        running = 0;
        std::cout << "failed to create server" << std::endl;
    }else{
        running = 1;
        std::cout << "created server on port "  << server->address.port << std::endl;
    }
}

void Server::DestroyServer(){
    enet_host_destroy(server);
}


void Server::Run(){
    while(running){
        ENetEvent event = {};
        
        while(enet_host_service(server, &event, 10) > 0){
            std::cout << "Event fired" << std::endl;
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "Client connected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                    Player player;
                    player.PlayerEntityID = entities.SpawnEntity(glm::vec3{64+clients.size(), 32, 64});
                    player.client = event.peer;
                    
                    clients.push_back(player);
                    
                    SendHandshake(&clients.back(), clients); 
                    SendSpawn(&clients.back(), clients);

                    std::cout << "Handshake sent" << std::endl;
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    {
                    std::cout << "received data in " << event.channelID << " of " << event.packet->dataLength << " size. Data:" <<  std::endl;
                    std::cout << event.packet->data  << std::endl;
                    
                    std::string data((char*)event.packet->data);    
                    if(data.front() == 'm'){
                        SendMovement(event.peer, clients, data);
                    }

                    enet_packet_destroy(event.packet);
                    break;   
                    }
                case ENET_EVENT_TYPE_DISCONNECT: 
                    std::cout << "Client disconnected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                    i32 index = SendDisconnect(event.peer, clients);
                    clients.erase(clients.begin() + index);
                    break;
            }
        }
    }
}


void Server::Stop(){
    running = 0;
}

void Server::SendHandshake(Player* client, std::vector<Player> clients){
    EntityData* player = entities.GetEntity(client->PlayerEntityID);
    std::string data = "h";
    data += "i" + std::to_string(client->PlayerEntityID);
    data += "p" + std::to_string((int)player->position.x) + " " + std::to_string((int)player->position.y) + " " + std::to_string((int)player->position.z);
    if(clients.size() > 1){
        data += "o" + std::to_string(clients.size()-1);
        for(i32 i = 0; i < clients.size(); i++){
            if(clients[i].client == client->client){
                continue;
            }else{
                EntityData* entity = entities.GetEntity(clients[i].PlayerEntityID);
                data+= "i" + std::to_string(clients[i].PlayerEntityID); 
                data+= "p" + std::to_string((int)entity->position.x) + " " + std::to_string((int)entity->position.y) + " " + std::to_string((int)entity->position.z);
            
            }
        }
    }
    data += "e";

    SendMessage(data.c_str(), data.size()+1, client->client);
}


i32 Server::SendDisconnect(ENetPeer* client, std::vector<Player> clients){
    i32 disconnectIndex;
    std::string data = "d";

    for(int i = 0; i < clients.size(); i++){
        if(clients[i].client == client){
            disconnectIndex = i;
            break;
        }
    }

    for(int i = 0; i < clients.size(); i++){
        if(clients[i].client != client){
            data += std::to_string(clients[disconnectIndex].PlayerEntityID);
            SendMessage(data.c_str(), data.size()+1, clients[i].client);
        }
    }

    return disconnectIndex;
}

void Server::SendSpawn(Player* client, std::vector<Player> clients){
    EntityData* player = entities.GetEntity(client->PlayerEntityID);
    std::string data = "s";
    data += "i" + std::to_string(client->PlayerEntityID);
    data += "p" + std::to_string((int)player->position.x) + " " + std::to_string((int)player->position.y) + " " + std::to_string((int)player->position.z);
    for(int i = 0; i < clients.size(); i++){
        if(clients[i].client != client->client){
            SendMessage(data.c_str(), data.size()+1, clients[i].client);
        }
    }
}
    

void Server::SendMovement(ENetPeer* client, std::vector<Player> clients, std::string data){
    for(i32 i = 0; i < clients.size(); i++){
        if(clients[i].client == client){
            EntityData* entity = entities.GetEntity(clients[i].PlayerEntityID);
        }else{
                SendMessage(data.c_str(), data.size()+1, clients[i].client);
        }
    }    
}