#pragma once
#include "depedencies.h"
#include "enet/enet.h"


void SendMessage(const char* data, size_t dataSize, ENetPeer* to);