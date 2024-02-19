#pragma once
#include "depedencies.h"
#include "entity_data.h"
#include "mesh.h"
#include "camera.h"
#include "vertices_list.h"
#include <vector>
#include <map>


class EntityMesh{
    Mesh mesh;
    std::vector<f32> vertices;
    std::vector<f32> instanceData;
    std::vector<u32> indices;
    u32 instanceCount;
    u8 initialized = 0;

public:
    void InitializeEntityMesh();
    void DestroyEntityMesh();
    void Update(std::map<u32, EntityData>* entities);
    void Draw(Camera* camera);
};