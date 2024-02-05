#pragma once
#include "depedencies.h"
#include "vertices_list.h"
#include "item_data.h"
#include "camera.h"
#include "mesh.h"
#include <map>


class ItemMesh{
private:    
    std::vector<f32> vertices;
    std::vector<f32> instanceData;
    std::vector<u32> indices;
    glm::mat4 transform = glm::mat4(1.0f);
    i32 indicesCount = 0;
    Mesh mesh;
public:
    void InitializeMesh(std::map<u32, u8>* itemInstances, std::map<u8, ItemData>* items);
    void Draw(Camera* camera);
};