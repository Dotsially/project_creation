#pragma once
#include "depedencies.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>


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

    void InitializeMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
    void AddAttribute(i32 dataSize, i32 stride, i32 offset);
    void SendData(f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);

    void AddInstanceData(i32 drawType, f32* verticesData, i32 verticesDataSize);
    void AddInstanceAttribute(i32 dataSize, i32 stride, i32 offset);
    void SendInstanceData(i32 drawType, f32* verticesData, i32 verticesDataSize);

    void DrawMesh(i32 indicesSize);
    void DrawInstancedMesh(i32 indicesSize, i32 instances);
};