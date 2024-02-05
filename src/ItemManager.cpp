#include "item_manager.h"
#include <iostream>

ItemManager::ItemManager(std::map<std::string, glm::vec2>* textures){
    sekaiReader.ReadItems(&items, textures);
}

ItemManager::~ItemManager(){}

void ItemManager::SetupItems(i32 itemCount, std::vector<Room>* rooms){
    for(int i = 0; i < itemCount; i++){
        u32 roomIndex = rand() % rooms->size();
        Room room = rooms->at(roomIndex);

        u8 item = rand () % items.size(); 
        u8 success = 0;
        while(!success){
            u32 x = room.position.x + (rand() % room.width);
            u32 y = room.position.y + (rand() % room.height);  
            if(!itemInstances.count(y + x*64)){
                itemInstances[y + x*64] = item;
                success = 1;
            }
        }
    }

}

void ItemManager::CreateItemMesh(){
    meshes.InitializeMesh(&itemInstances, &items);
}

void ItemManager::DrawItems(Camera* camera){
    meshes.Draw(camera);
}

std::map<u8, ItemData>* ItemManager::GetItems(){
    return &items;
}