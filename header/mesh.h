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
    u32 ebo;
    i32 drawType;
public:
    Mesh();
    ~Mesh();

    void InitializeChunkMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
    void SendChunkData(f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
    void DrawChunk(i32 indicesSize, glm::vec3 position);

    void InitializeMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
    void DrawMesh(i32 indicesSize, glm::mat4 transform);

    //TODO
    void InitializeInstancedMesh();
    void DrawInstancedMesh();

    void InitializeEntityMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
};