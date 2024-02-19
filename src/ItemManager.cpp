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
            u32 x = room.position.x + 2 + (rand() % (room.width-2));
            u32 y = room.position.y + 1 + (rand() % (room.height-2));  

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
    meshes.Draw(camera, itemInstances.size());
}

std::map<u8, ItemData>* ItemManager::GetItems(){
    return &items;
}