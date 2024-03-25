#pragma once
#include "depedencies.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

typedef struct ChunkVertex{
    u32 packedPositionAndUv;
    f32 face;   
} ChunkVertex;


class Mesh{
    u32 vao;
    u32 vbo;
    u32 vboInstanced;
    u32 ebo;
    i32 drawType;
    u8 attributeCount = 0;
public:
    Mesh();
    ~Mesh();

    void InitializeMesh(i32 drawType, void* verticesData, i32 verticesDataSize, i32 vertexSize, u32* indicesData, i32 indicesDataSize);
    void AddAttribute(i32 attributeType, i32 attributeSize, i32 dataSize, i32 stride, i32 offset);
    void AddIntAttribute(i32 attributeType, i32 attributeSize, i32 dataSize, i32 stride, i32 offset);
    void SendData(void* verticesData, i32 verticesDataSize, i32 vertexSize, u32* indicesData, i32 indicesDataSize);

    void AddInstanceData(i32 drawType, f32* verticesData, i32 verticesDataSize);
    void AddInstanceAttribute(i32 dataSize, i32 stride, i32 offset);
    void SendInstanceData(i32 drawType, f32* verticesData, i32 verticesDataSize);

    void DrawMesh(i32 indicesSize);
    void DrawInstancedMesh(i32 indicesSize, i32 instances);
};