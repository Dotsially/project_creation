#include "server/networking.h"

void SendMessage(const char* data, size_t dataSize, ENetPeer* to){
    ENetPacket* packet = enet_packet_create(data, dataSize, ENET_PACKET_FLAG_RELIABLE);\
    
    //second param is channel id
    enet_peer_send(to, 0, packet);
}