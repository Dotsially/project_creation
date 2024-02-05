#include "dungeon.h"
#include <iostream>


void Dungeon::CreateDungeonFloor(){
    i32 dungeonSize = 64;
    dungeonMap = (u8*)calloc(dungeonSize*dungeonSize, 1);

    BSPTree tree;
    tree.CreateBSP(dungeonSize-1,dungeonSize-1, 20);
    //tree.PrintTree();
    for(i32 i = 20; i < tree.GetLeaves().size(); i++){
        if(rand() % 3 > 1){
            Room room;
            room.type = 0;
            room.position = tree.GetLeaves()[i].data.startPosition + 1;
            room.width = tree.GetLeaves()[i].data.width -1;
            room.height = tree.GetLeaves()[i].data.height -1;
            room.center = glm::ivec2(room.position.x + room.width/2.0, room.position.y + room.height/2.0);
            rooms.push_back(room);
        }
        
    }

    for(i32 x = 0; x < dungeonSize; x++){
        for(i32 y = 0; y < dungeonSize; y++){
            dungeonMap[y + x*dungeonSize] = 3;
        }
    }

    for(Room room : rooms){
        i32 startX = room.position.x;
        i32 endX = room.position.x + room.width;
        i32 startY = room.position.y;
        i32 endY = room.position.y + room.height; 

        for(i32 x = startX; x < endX; x++){
            for(i32 y = startY; y < endY; y++){
                dungeonMap[y + x*dungeonSize] = 0;
            }
        }
        
    }
        
    for(int i = 1; i < rooms.size(); i++){
        glm::ivec2 room1 = rooms[i-1].center;
        glm::ivec2 room2 = rooms[i].center;

        glm::ivec2 difference = room2 - room1;
        if(difference.x < 0){
            for(int x = room1.x; x > room2.x; x--){
                dungeonMap[room1.y + x * dungeonSize] = 0;
            }
        }else{
            for(int x = room1.x; x < room2.x; x++){
                dungeonMap[room1.y + x * dungeonSize] = 0;
            }
        }
        
        if(difference.y < 0){
            for(int y = room1.y; y > room2.y; y--){
                dungeonMap[y + room2.x * dungeonSize] = 0;
            }
        }
        else{
            for(int y = room1.y; y < room2.y; y++){
                dungeonMap[y + room2.x * dungeonSize] = 0;
            }
        }
    }
}


void Dungeon::DestroyDungeonFloor(){
    free(dungeonMap);
}


u8* Dungeon::GetDungeonMap(){
    return dungeonMap;
}


std::vector<Room>* Dungeon::GetRooms(){
    return &rooms;
}