#pragma once
#include "depedencies.h"
#include "chunk.h"
#include "block_data.h"
#include "biome.h"
#include "world_data.h"

class World{
    BiomeData biome;
    Chunk* chunks;
    u32 REGION_SIZE = 16;
    SekaiReader sekaiReader;
    WorldData worldData;
public:
    World(std::map<u8, BlockData>* blocks, std::map<std::string, BlockModelData>* blockModels, Biome biomes);
    ~World();

    void Draw(glm::vec3 playerPos, glm::vec3 cameraPosition);
    void AddBlock(i32 x, i32 y, i32 z, BlockInstanceData block);
    void RemoveBlock(i32 x, i32 y, i32 z);
    u8 IsWithinWorld(i32 x, i32 y, i32 z);
    u8 IsWithinWorld(glm::vec3);
    u8 ContainsBlock(i32 x, i32 y, i32 z);
    void GetNeighbors(Chunk* neighbors[4], i32 x, i32 z);
    i32 GetTerrainHeight(i32 x, i32 z);
    glm::vec3 GetSkyColor();
};  