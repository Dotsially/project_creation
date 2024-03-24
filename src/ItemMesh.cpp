#include "item_mesh.h"
#include <iostream>



void ItemMesh::InitializeMesh(std::map<u32, u8>* itemInstances, std::map<u8, ItemData>* items){
    for(i32 i = 0; i < 4; i++){
            vertices.push_back(VerticesList::verticesEntity1x1[i*2]);
            vertices.push_back(VerticesList::verticesEntity1x1[1+i*2]);
            vertices.push_back(0);
    }
    
    for(auto item = itemInstances->begin(); item != itemInstances->end(); item++){
        i32 y = item->first % 64;
        i32 x = item->first / 64;
        instanceData.push_back(x);
        instanceData.push_back(1);
        instanceData.push_back(y);
        instanceData.push_back(items->at(item->second).texture.x);
        instanceData.push_back(items->at(item->second).texture.y);
        instanceData.push_back(1);
        instanceData.push_back(1);
    }

    for(u32 i : VerticesList::indicesEntity){
        indices.push_back(i);
    }

    mesh.InitializeMesh(GL_STATIC_DRAW, vertices.data(), vertices.size(), indices.data(), indices.size());
    mesh.AddAttribute(3, 3, 0);
    mesh.AddInstanceData(GL_STATIC_DRAW, instanceData.data(), instanceData.size());
    mesh.AddInstanceAttribute(3,7,0);
    mesh.AddInstanceAttribute(2,7,3);
    mesh.AddInstanceAttribute(2,7,5);
}

void ItemMesh::Draw(Camera* camera, i32 instances){
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::vec3 cameraRight = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glm::vec3 cameraUp = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
    
    glUniform3fv(3, 1, glm::value_ptr(cameraRight));
    glUniform3fv(4, 1, glm::value_ptr(cameraUp));
    
    mesh.DrawInstancedMesh(indices.size(), instances);
}