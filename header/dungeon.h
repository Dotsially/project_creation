#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"
#include <vector>
#include "binary_tree.h"
#include "block_manager.h"

typedef struct DungeonMap{
    u8 elevation;
    u8 blockType;
} DungeonMap;


typedef struct Room{
    u32 width;
    u32 height;
    glm::ivec2 position;
    glm::ivec2 center;
    u8 type = 0;
}Room;

class Dungeon{
    std::vector<Room> rooms;
    DungeonMap* dungeonMap;
public:
    void CreateDungeonFloor(BlockManager* blockManager);
    void DestroyDungeonFloor();
    DungeonMap* GetDungeonMap();
    std::vector<Room>* GetRooms();
};