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
    i32 indicesCount = 0;
    Mesh mesh;

    i32 CalculateBitTile(TileData* worldTiles, i32 width, i32 height, i32 x, i32 y);
public:
    void InitializeMesh(TexturePacker* overworldPacker, TileData* worldTiles, i32 width, i32 height);
    void Draw();
};