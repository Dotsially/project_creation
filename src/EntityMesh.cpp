#include "entity_mesh.h"
#include "iostream"

void EntityMesh::InitializeEntityMesh(){
    for(i32 i = 0; i < 4; i++){
        vertices.push_back(VerticesList::verticesEntity1x1[i*2]);
        vertices.push_back(VerticesList::verticesEntity1x1[1+i*2]);
        vertices.push_back(0);
    }

    for(u32 i : VerticesList::indicesEntity){
        indices.push_back(i);
    }

    mesh.InitializeMesh(GL_STATIC_DRAW, vertices.data(), vertices.size(), sizeof(f32), indices.data(), indices.size());    
    mesh.AddAttribute(GL_FLOAT, sizeof(f32), 3, 3, 0);
}

void EntityMesh::Update(std::map<u32, EntityData>* entities){
    instanceData.clear();
    for(auto entity = entities->begin(); entity != entities->end(); entity++){
        instanceData.push_back(entity->second.position.x);
        instanceData.push_back(entity->second.position.y);
        instanceData.push_back(entity->second.position.z);
        instanceData.push_back(entity->second.flipped);
        instanceData.push_back(entity->second.textureDirection);
        instanceData.push_back(1+1.0/16.0);
        instanceData.push_back(2);
    }
    instanceCount = entities->size();

    if(!initialized){
        initialized = 1;
        mesh.AddInstanceData(GL_DYNAMIC_DRAW, instanceData.data(), instanceData.size());
        mesh.AddInstanceAttribute(3, 7, 0);
        mesh.AddInstanceAttribute(2, 7, 3);
        mesh.AddInstanceAttribute(2, 7, 5);
    }
    else{
        mesh.SendInstanceData(GL_DYNAMIC_DRAW, instanceData.data(), instanceData.size());
    }
}

void EntityMesh::Draw(Camera* camera){
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::vec3 cameraRight = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glm::vec3 cameraUp = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
 
    glUniform3fv(3, 1, glm::value_ptr(cameraRight));
    glUniform3fv(4, 1, glm::value_ptr(cameraUp));

    mesh.DrawInstancedMesh(indices.size(), instanceCount);
}