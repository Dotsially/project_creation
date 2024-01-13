#include "chunk_pool.h"
#include <iostream>

void ChunkPool::InitializeChunkPool(i32 renderDistance){
    chunkMeshes = new ChunkMesh[(renderDistance*2+1)*(renderDistance*2+1) + (renderDistance*2+1)];
    chunkPoolSize = (renderDistance*2+1)*(renderDistance*2+1) + (renderDistance*2+1);
}

void ChunkPool::DeleteChunkPool(){
    delete(chunkMeshes);
}


ChunkMesh* ChunkPool::GetAvailableChunkMesh(){
    std::cout << "MAX:" << chunkPoolSize  << std::endl;
    std::cout << "IN USE:" << chunkPoolInUse << std::endl;
    for(int i = 0; i < chunkPoolSize; i++){
        if(!chunkMeshes[i].inUse){
            chunkMeshes[i].inUse = 1;
            chunkPoolInUse++;
            return &chunkMeshes[i];
            break;           
        }
    }
    std::cout << "null" << std::endl;
    return nullptr;
}

void ChunkPool::FreeInUseMesh(ChunkMesh* chunkMesh){
    if(chunkMesh != nullptr){
        chunkMesh->inUse = 0;
        chunkPoolInUse--;
    }
}