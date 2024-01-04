#include "entity_mesh.h"
#include "iostream"

EntityMesh::EntityMesh(){
    mesh = Mesh();

    for(i32 i = 0; i < 4; i++){
        vertices.push_back(VerticesList::verticesEntity1x1[0+(i*4)]);
        
        vertices.push_back(VerticesList::verticesEntity1x1[1+(i*4)]);
    }

    for(u32 i : VerticesList::indicesEntity){
                indices.push_back(i);
    }


    mesh.InitializeMesh(GL_STATIC_DRAW, vertices.data(), vertices.size(), indices.data(), indices.size());

}

EntityMesh::~EntityMesh(){

}

void EntityMesh::Update(glm::vec3 position){

    for(i32 i = 0; i < 4; i++){
        vertices.push_back(VerticesList::verticesEntity1x1[0+(i*2)] + position.x);
        
        vertices.push_back(VerticesList::verticesEntity1x1[1+(i*2)] + position.y);
    }

    for(u32 i : VerticesList::indicesEntity){
                indices.push_back(i);
    }

    mesh.SendChunkData(vertices.data(), vertices.size(), indices.data(), indices.size());

}

void EntityMesh::Draw(glm::vec3 position, Camera* camera){
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::vec3 cameraRight = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glm::vec3 cameraUp = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
    glm::vec3 finalPosition = glm::vec3(position.x+0.5f, position.y+1.0f, position.z);
    //transform = glm::translate(glm::mat4(1.0f), finalPosition);
    //transform = glm::scale(transform, glm::vec3(1,2,1));

    glUniform3fv(3, 1, glm::value_ptr(finalPosition));
    glUniform3fv(4, 1, glm::value_ptr(cameraRight));
    glUniform3fv(5, 1, glm::value_ptr(cameraUp));

    mesh.DrawMesh(indices.size(), transform);
}