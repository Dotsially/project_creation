#pragma once
#include "depedencies.h"
#include "mesh.h"



typedef struct ChunkMesh{
    Mesh mesh;
    std::vector<ChunkVertex> vertices;
    std::vector<u32> indices;
    u32 indicesCount;
    u8 inUse = 0;
}ChunkMesh;

class ChunkPool{
private:    
    ChunkMesh* chunkMeshes;
    i32 chunkPoolSize;
    i32 chunkPoolInUse = 0;
public:    
    void InitializeChunkPool(i32 renderDistance);
    void DeleteChunkPool();

    ChunkMesh* GetAvailableChunkMesh();
    void FreeInUseMesh(ChunkMesh* chunkMesh);
};