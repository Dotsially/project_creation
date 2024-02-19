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
public:
    Mesh();
    ~Mesh();

    void InitializeChunkMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
    void SendChunkData(f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
    void DrawChunk(i32 indicesSize, glm::vec3 position, glm::vec3 cameraPosition, glm::vec3 fogColor);

    void InitializeMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
    void InitializeBillboardMesh(i32 drawType, f32* verticesData, i32 verticesDataSize);
    void DrawBillboardMesh(i32 indicesSize);
    void DrawMesh(i32 indicesSize, glm::mat4 transform);

    //TODO
    void InitializeInstancedMesh(i32 drawType, f32* verticesData, i32 verticesDataSize);
    void AddBillboardInstanceData(i32 drawType, f32* verticesData, i32 verticesDataSize);
    void SendBillboardData(i32 drawType, f32* verticesData, i32 verticesDataSize);
    void DrawInstancedBillboard(i32 indicesSize, i32 instances);
    

    void InitializeItemMesh(i32 drawType, f32* verticesData, i32 verticesDataSize, u32* indicesData, i32 indicesDataSize);
};