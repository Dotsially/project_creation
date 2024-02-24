#include "mesh.h"
#include <iostream>


Mesh::Mesh(){
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboInstanced);
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

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::SendChunkData(f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize){
    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize*sizeof(u32), indicesData, drawType);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::DrawChunk(i32 indicesSize, glm::vec3 position, glm::vec3 cameraPosition, glm::vec3 fogColor){
    glm::mat4 transform = glm::translate(glm::mat4(1), position);
    glBindVertexArray(vao);
    glUniformMatrix4fv(0,1, false, glm::value_ptr(transform));
    glUniform3fv(3, 1, glm::value_ptr(cameraPosition));
    glUniform3fv(4, 1, glm::value_ptr(fogColor));
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::InitializeMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize){
    this->drawType = drawType;
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize*sizeof(u32), indicesData, drawType);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::AddAttribute(i32 dataSize, i32 stride, i32 offset){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(attributeCount, dataSize, GL_FLOAT, GL_FALSE, stride*sizeof(f32), (void*)(offset*sizeof(f32)));
    glEnableVertexAttribArray(attributeCount);

    attributeCount++;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::AddBillboardInstanceData(i32 drawType, f32* verticesData, i32 verticesDataSize){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstanced);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);
    
    i32 first = attributeCount;
    i32 second = attributeCount+1;
    i32 third = attributeCount+2;

    glEnableVertexAttribArray(first);   
    glEnableVertexAttribArray(second);
    glEnableVertexAttribArray(third);

    glVertexAttribPointer(first, 3, GL_FLOAT, GL_FALSE, 7*sizeof(f32), (void*)0);
    glVertexAttribPointer(second, 2, GL_FLOAT, GL_FALSE, 7*sizeof(f32), (void*)(3*sizeof(f32)));
    glVertexAttribPointer(third, 2, GL_FLOAT, GL_FALSE, 7*sizeof(f32), (void*)(5*sizeof(f32)));
    
    glVertexAttribDivisor(first, 1);
    glVertexAttribDivisor(second, 1);
    glVertexAttribDivisor(third, 1);

    attributeCount += 3;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::SendBillboardData(i32 drawType, f32* verticesData, i32 verticesDataSize){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstanced);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
}

void Mesh::DrawMesh(i32 indicesSize, glm::mat4 transform){
    glBindVertexArray(vao);
    glUniformMatrix4fv(0,1, false, glm::value_ptr(transform));
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::DrawInstancedBillboard(i32 indicesSize, i32 instances){
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0, instances);
    glBindVertexArray(0);
}