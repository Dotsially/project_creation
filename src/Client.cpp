#include "client.h"
#include <iostream>


void Client::InitializeClient(){
    client = enet_host_create(NULL, 1, 2, 0,0);
                if(client == NULL){
                    std::cout << "failed to create client" << std::endl; 
                }
                else{
                    std::cout << "successfully created client" << std::endl;
                }
}


void Client::DestroyClient(){
    if(server != nullptr){
        enet_peer_reset(server);
    }
    enet_host_destroy(client);
}

void Client::Update(World* world, EntityManager* entities){
    ENetEvent event = {};

    while(enet_host_service(client, &event, 0) > 0){
        switch(event.type){
            case ENET_EVENT_TYPE_RECEIVE:
                std::string data((char*)event.packet->data);    
                if(data.front() == 'd'){
                    i32 entityID = std::stoi(data.substr(1));
                    entities->DestroyEntity(entityID);
                }
                if(data.front() == 's'){
                    i32 i = data.find("i");
                    i32 p = data.find("p");

                    std::string id = data.substr(i+1, p-i-1);
                    std::string pos = data.substr(p+1);

                    SpawnEntity(entities, world, pos, id, 0);
                }
                if(data.front() == 'm'){
                    i32 i = data.find("i");
                    i32 p = data.find("p");
                    i32 d = data.find("d");

                    std::string id = data.substr(i+1, p-i-1);
                    std::string pos = data.substr(p+1, d - p - 1);
                    
                    EntityData* entity = entities->GetEntity(std::stoi(id));
                    
                    i32 first = pos.find(" ");
                    i32 second = pos.substr(first+1).find(" ");
    
                    i32 x = std::stoi(pos.substr(0, first));
                    i32 y = std::stoi(pos.substr(first+1));
                    i32 z = std::stoi(pos.substr(first+second+2));
                    
                    entity->position = glm::vec3{x,y,z};
                    entity->moveDirectionValue = std::stoi(data.substr(d+1));
                }

                enet_packet_destroy(event.packet);
                break;
        }
    }
}

void Client::Connect(std::string ip, i32 port){
    enet_address_set_host(&address, ip.c_str());
    address.port = port;
    server = enet_host_connect(client, &address, 2, 0);
    if(server == nullptr){
        std::cout << "failed to connect to server" << std::endl;
        enet_host_destroy(client);
    }
    else{
        ENetEvent event = {};
        if(enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT){
            std::cout << "connected to server" << std::endl;
            enet_host_flush(client);
            connected = 1;
        }
    }
}

void Client::SendMessageServer(std::string message){
    SendMessage(message.c_str(), message.size()+1, server);
}

void Client::ReceiveHandshake(World* world, EntityManager* entities){
    ENetEvent event = {};

    while(enet_host_service(client, &event, 100) > 0){
        switch(event.type){
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "handshake received:" << std::endl;
                std::cout << event.packet->data << std::endl;  
                std::string data((char*)event.packet->data);    
                if(data.front() == 'h'){
                    i32 i = data.find("i");
                    i32 p = data.find("p");
                    i32 o = data.find("o");
                    i32 e = data.find("e");
                    
                    std::string id = data.substr(i+1, p-i-1);
                    std::string pos;

                    if(o > 0){
                        pos = data.substr(p+1, o - p - 1);
                    
                        SpawnEntity(entities, world, pos, id, 1);  

                        i32 pc = data.substr(o).find("i");
                        i32 playerCount = std::stoi(data.substr(o+1, pc - o - 1)); 
                        
                        i32 idloc = data.substr(o).find("i");
                        std::string otherPlayers = data.substr(o + idloc, e - idloc - 1);
                        for(i32 i = 0; i < playerCount; i++){
                            i32 idInfoEnd = otherPlayers.find("p");
                            i32 playerInfoEnd = otherPlayers.substr(1).find("i");

                            id = otherPlayers.substr(1, idInfoEnd-1);
                            std::cout << id << std::endl;
                            if(playerInfoEnd > 0){
                                std::string pos = otherPlayers.substr(idInfoEnd+1, playerInfoEnd-1);
                                SpawnEntity(entities, world, pos, id, 0); 
                                
                                std::cout << pos << std::endl;
                                otherPlayers = otherPlayers.substr(1,playerInfoEnd-1);
                            }
                            else{
                                std::string pos = otherPlayers.substr(idInfoEnd+1);
                                
                                std::cout << pos << std::endl;
                                SpawnEntity(entities, world, pos, id, 0); 
                            }

                        }
                    }
                    else{
                        pos = data.substr(p+1, e - p -1);
                        SpawnEntity(entities, world, pos, id, 1);  
                    }
                    
                }         
                
                enet_packet_destroy(event.packet); 
                break;
        }
    }    
}


void Client::SpawnEntity(EntityManager* entities, World* world, std::string pos, std::string id, i32 type){
    i32 first = pos.find(" ");
    i32 second = pos.substr(first+1).find(" ");
    
    i32 x = std::stoi(pos.substr(0, first));
    i32 y = std::stoi(pos.substr(first+1));
    i32 z = std::stoi(pos.substr(first+second+2));
    
    i32 idValue = std::stoi(id);
    glm::vec3 position = glm::vec3{x,y,z};

    if(type == 0){
        entities->SpawnEntity(position, idValue);
    }
    else{
        entities->SpawnPlayer(glm::vec2{position.x, position.z}, idValue, world); 
    }
}

ENetPeer* Client::GetServer(){
    return server;
}