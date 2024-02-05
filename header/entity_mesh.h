#pragma once
#include "depedencies.h"
#include "entity.h"
#include "mesh.h"
#include "camera.h"
#include "vertices_list.h"
#include <vector>


class EntityMesh{
    std::vector<f32> vertices;
    std::vector<u32> indices;
    Mesh mesh;

public:
    EntityMesh();
    ~EntityMesh();
    void Update(glm::vec3 position);
    void Draw(Entity* entity, Camera* camera);
};