#pragma once
#include "depedencies.h"
#include "mesh.h"
#include "vertices_list.h"
#include "tile_data.h"
#include "texture_packer.h"


class OverworldMesh{
private:
    std::vector<f32> vertices;
    std::vector<u32> indices;
    f32 waterVertices[12];
    u32 waterIndices[6];
    i32 indicesCount = 0;
    Mesh mesh;
    Mesh waterMesh;
    const glm::vec3 colors[6] = {glm::vec3{0.922, 0.929, 0.914}, glm::vec3{0.643, 0.867, 0.859}, glm::vec3{0.459, 0.655, 0.263}, glm::vec3{0.275, 0.51, 0.196}, glm::vec3{0.906, 0.835, 0.702}, glm::vec3{0.91, 0.757, 0.439},  };
    i32 MarchingSquares(TileData* worldTiles, i32 width, i32 height, i32 x, i32 y);
    glm::vec4 Subblob(TileData* worldTiles, i32 width, i32 height, i32 x, i32 y);
    void GenerateMesh(TexturePacker* overworldPacker, TileData* worldTiles, i32 width, i32 height, i32 x, i32 y);
public:
    void InitializeMesh(TexturePacker* overworldPacker, TileData* worldTiles, i32 width, i32 height, i32 screenWidth, i32 screenHeight);
    void ReloadMesh(TexturePacker* overworldPacker, TileData* worldTiles, i32 width, i32 height);
    void Draw();
    void DrawWater();
};