#pragma once
#include "depedencies.h"
#include "FastNoiseLite.h"
#include "vertices_list.h"
#include <vector>
#include <map>
#include <string>
#include "mesh.h"
#include "block_data.h"
#include "biome_data.h"
#include "chunk_pool.h"

#define CHUNK_SIZE 32
#define CHUNK_HEIGHT 128

class Chunk{
    glm::vec2 position;
    BlockInstanceData chunkData[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    std::map<u8, BlockData>* blocks;
    std::map<std::string, BlockModelData>* blockModels;
    ChunkMesh* chunkMesh = nullptr;
    Chunk* chunkNeighbors[4];

    void AddFace(u32 faceIndex, glm::vec3 position);
    void CreateBlock(Chunk** chunks, i32 x, i32 y, i32 z);
public:
    Chunk();
    ~Chunk();
    void CreateChunkData(std::map<u8, BlockData>* blocks, std::map<std::string, BlockModelData>* blockModels, BiomeData biome, std::vector<fnl_state> noise, i32 x, i32 z); 
    void CreateChunkMesh(Chunk** chunks);
    void SendMeshData();
    void Draw(glm::vec3 cameraPosition, glm::vec3 fogColor);
    void AddBlock(Chunk** chunks, i32 x, i32 y, i32 z, BlockInstanceData block);
    void RemoveBlock(Chunk** chunks, i32 x, i32 y, i32 z);
    void RequestAvailableChunkMesh(ChunkMesh* chunkMesh);
    void DisposeChunkMesh();
    u32 GetChunkMeshSize();
    u32 GetChunkIndicesSize();
    f32* GetChunkMeshData();
    u32* GetChunkIndicesData();
    ChunkMesh* GetChunkMesh();
    bool ConstainsBlock(i32 x, i32 y, i32 z);
    bool IsWithinChunk(i32 x, i32 y, i32 z);
    glm::vec2 GetPosition();
    i32 GetCoordinateTerrainHeight(i32 x, i32 z);
    
    u8 hasChunkMesh = 0;
};