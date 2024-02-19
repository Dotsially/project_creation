#pragma once
#include "depedencies.h"
#include "enet/enet.h"

typedef struct Player{
    ENetPeer* client;
    i32 PlayerEntityID;
}Player;