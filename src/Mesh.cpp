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


void Mesh::InitializeMesh(i32 drawType, void* verticesData, i32 verticesDataSize, i32 vertexSize, u32* indicesData, i32 indicesDataSize){
    this->drawType = drawType;
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*vertexSize, verticesData, drawType);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize*sizeof(u32), indicesData, drawType);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::AddAttribute(i32 attributeType, i32 attributeSize, i32 dataSize, i32 stride, i32 offset){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glVertexAttribPointer(attributeCount, dataSize, attributeType, GL_FALSE, stride*attributeSize, (void*)(offset*attributeSize));
    glEnableVertexAttribArray(attributeCount);
    attributeCount++;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::AddIntAttribute(i32 attributeType, i32 attributeSize, i32 dataSize, i32 stride, i32 offset){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    
    glVertexAttribIPointer(attributeCount, dataSize, attributeType, stride*attributeSize, (void*)(offset*attributeSize));
    glEnableVertexAttribArray(attributeCount);
    attributeCount++;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::SendData(void* verticesData, i32 verticesDataSize, i32 vertexSize, u32* indicesData, i32 indicesDataSize){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, verticesDataSize*vertexSize, verticesData, drawType);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize*sizeof(u32), indicesData, drawType);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::AddInstanceData(i32 drawType, f32* verticesData, i32 verticesDataSize){
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vboInstanced);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::AddInstanceAttribute(i32 dataSize, i32 stride, i32 offset){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstanced);

    glVertexAttribPointer(attributeCount, dataSize, GL_FLOAT, GL_FALSE, stride*sizeof(f32), (void*)(offset*sizeof(f32)));
    glEnableVertexAttribArray(attributeCount);
    glVertexAttribDivisor(attributeCount, 1);
    attributeCount++;
    

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::SendInstanceData(i32 drawType, f32* verticesData, i32 verticesDataSize){
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vboInstanced);
    glBufferData(GL_ARRAY_BUFFER, verticesDataSize*sizeof(f32), verticesData, drawType);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
}

void Mesh::DrawMesh(i32 indicesSize){
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::DrawInstancedMesh(i32 indicesSize, i32 instances){
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0, instances);
    glBindVertexArray(0);
}