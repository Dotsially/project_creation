#include "dungeon.h"
#include <iostream>


void Dungeon::CreateDungeonFloor(BlockManager* blockManager){
    i32 dungeonSize = 64;
    dungeonMap = (DungeonMap*)calloc(dungeonSize*dungeonSize, sizeof(DungeonMap));

    BSPTree tree;
    tree.CreateBSP(dungeonSize,dungeonSize, 20);
    //tree.PrintTree();
    for(i32 i = 20; i < tree.GetLeaves().size(); i++){
        if(rand() % 3 > 1){
            Room room;
            room.type = 0;
            room.position = tree.GetLeaves()[i].data.startPosition;
            room.width = tree.GetLeaves()[i].data.width;
            room.height = tree.GetLeaves()[i].data.height;
            room.center = glm::ivec2(room.position.x + room.width/2.0, room.position.y + room.height/2.0);
            rooms.push_back(room);
        }
        
    }

    for(i32 x = 0; x < dungeonSize; x++){
        for(i32 y = 0; y < dungeonSize; y++){
            dungeonMap[y + x*dungeonSize] = DungeonMap{3,blockManager->BlockNameToBlockId("dungeon_wall")};
        }
    }

    for(Room room : rooms){
        i32 startX = room.position.x;
        i32 endX = room.position.x + room.width;
        i32 startY = room.position.y;
        i32 endY = room.position.y + room.height; 

        for(i32 x = startX; x < endX; x++){
            for(i32 y = startY; y < endY; y++){
                if(y == startY || y == endY-1){
                    dungeonMap[y + x*dungeonSize] = DungeonMap{3,blockManager->BlockNameToBlockId("stone_brick")};
                }
                else {
                    dungeonMap[y + x*dungeonSize] = DungeonMap{0,blockManager->BlockNameToBlockId("cobble_stone")};
                }

                if(x == startX || x == endX-1){
                    dungeonMap[y + x*dungeonSize] = DungeonMap{3,blockManager->BlockNameToBlockId("stone_brick")};
                }
            }
        }
        
    }
    
    //dungeonMap[0, 3 * 64] = DungeonMap{3,blockManager->BlockNameToBlockId("stone_brick")};
        
    for(int i = 1; i < rooms.size(); i++){
        glm::ivec2 room1 = rooms[i-1].center;
        glm::ivec2 room2 = rooms[i].center;

        glm::ivec2 difference = room2 - room1;
        if(difference.x < 0){
            for(int x = room1.x; x > room2.x; x--){
                if(!dungeonMap[room1.y + x * dungeonSize].elevation == 0){
                    dungeonMap[room1.y + x * dungeonSize] = DungeonMap{0,blockManager->BlockNameToBlockId("dungeon_floor")};
                }
            }
        }else{
            for(int x = room1.x; x < room2.x; x++){
                if(!dungeonMap[room1.y + x * dungeonSize].elevation == 0){
                    dungeonMap[room1.y + x * dungeonSize] = DungeonMap{0,blockManager->BlockNameToBlockId("dungeon_floor")};
                }
            }
        }
        
        if(difference.y < 0){
            for(int y = room1.y; y > room2.y; y--){
                if(!dungeonMap[y + room2.x * dungeonSize].elevation == 0){
                    dungeonMap[y + room2.x * dungeonSize] = DungeonMap{0,blockManager->BlockNameToBlockId("dungeon_floor")};
                }
            }
        }
        else{
            for(int y = room1.y; y < room2.y; y++){
            if(!dungeonMap[y + room2.x * dungeonSize].elevation == 0){
                dungeonMap[y + room2.x * dungeonSize] = DungeonMap{0,blockManager->BlockNameToBlockId("dungeon_floor")};
            }
            }
        }
    }
}


void Dungeon::DestroyDungeonFloor(){
    free(dungeonMap);
}


DungeonMap* Dungeon::GetDungeonMap(){
    return dungeonMap;
}


std::vector<Room>* Dungeon::GetRooms(){
    return &rooms;
}