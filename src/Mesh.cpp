#include "mesh.h"
#include <iostream>


Mesh::Mesh(){
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);    
}

Mesh::~Mesh(){ 
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::InitializeChunkMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize){
    this->drawType = drawType;
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize*sizeof(u32), indicesData, drawType);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(f32), 0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7*sizeof(f32), (void*)(3*sizeof(f32)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7*sizeof(f32), (void*)(4*sizeof(f32)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 7*sizeof(f32), (void*)(5*sizeof(f32)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
}

void Mesh::SendChunkData(f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize*sizeof(u32), indicesData, drawType);
}

void Mesh::DrawChunk(i32 indicesSize, glm::vec3 position){
    glm::mat4 transform = glm::translate(glm::mat4(1), position);
    glBindVertexArray(vao);
    glUniformMatrix4fv(0,1, false, glm::value_ptr(transform));
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

void Mesh::InitializeMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize){
    this->drawType = drawType;
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize*sizeof(u32), indicesData, drawType);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(f32), 0);
    glEnableVertexAttribArray(0);
}

void Mesh::InitializeEntityMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize){
    this->drawType = drawType;
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize*sizeof(u32), indicesData, drawType);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(f32), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(f32), (void*)(2*sizeof(f32)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Mesh::DrawMesh(i32 indicesSize, glm::mat4 transform){
    glBindVertexArray(vao);
    glUniformMatrix4fv(0,1, false, glm::value_ptr(transform));
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}