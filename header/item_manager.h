#pragma once
#include "depedencies.h"
#include "item_data.h"
#include "sekai_reader.h"
#include "dungeon.h"
#include "item_mesh.h"
#include <map>
#include <string>

class ItemManager{
private:
    std::map<u8, ItemData> items;
    std::map<u32, u8> itemInstances;
    SekaiReader sekaiReader;
    ItemMesh meshes; 
public:
    ItemManager(std::map<std::string, glm::vec2>* textures);
    ~ItemManager();
    
    void SetupItems(i32 itemCount, std::vector<Room>* rooms);
    void CreateItemMesh();
    void DrawItems(Camera* camera);
    std::map<u8, ItemData>* GetItems();

};