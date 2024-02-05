#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"
#include <vector>
#include "binary_tree.h"

typedef struct Room{
    u32 width;
    u32 height;
    glm::ivec2 position;
    glm::ivec2 center;
    u8 type = 0;
}Room;

class Dungeon{
    std::vector<Room> rooms;
    u8* dungeonMap;
public:
    void CreateDungeonFloor();
    void DestroyDungeonFloor();
    u8* GetDungeonMap();
    std::vector<Room>* GetRooms();
};