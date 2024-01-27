#include "chunk.h"
#include <iostream>

Chunk::Chunk(){
}

Chunk::~Chunk(){

}


void Chunk::CreateChunkData(std::map<u8, BlockData>* blocks, std::map<std::string, BlockModelData>* blockModels, BiomeData biome, std::vector<fnl_state> noise, i32 x, i32 z){
    this->blocks = blocks;
    this->blockModels = blockModels;
    position = glm::vec2(x,z);
    chunkCenter = glm::vec3((position.x+0.5)*CHUNK_SIZE,
        CHUNK_HEIGHT*0.5,
        (position.y+0.5)*CHUNK_SIZE);

    for(i32 x = 0; x < CHUNK_SIZE; x++){
        for(i32 z = 0; z < CHUNK_SIZE; z++){
            i32 value = biome.elevation + biome.height;
            if(noise.size() > 0){
                value = 0;
                for(int i = 0; i < noise.size(); i++){
                    i32 noiseValue = (fnlGetNoise2D(&noise[i], x + (position.x*CHUNK_SIZE), z + (position.y*CHUNK_SIZE))+1)*biome.height+biome.elevation;   
                    if(biome.noise[i].divisor > 0){
                        noiseValue = noiseValue/biome.noise[i].divisor;
                    }
                    value += noiseValue;
                }   
            }

            for(i32 y = 0; y < CHUNK_HEIGHT; y++){  
                if(y == value){
                    chunkData[x][y][z] = {biome.biomeLayersBlockID[0],0,0,0};
                }
                else if(y < value && y > value-2){
                    chunkData[x][y][z] = {biome.biomeLayersBlockID[1],0,0,0};
                }
                else if(y <= value-2 || y == 0){
                    chunkData[x][y][z] = {biome.biomeLayersBlockID[2],0,0,0};
                }
                else{
                    chunkData[x][y][z] = {0,0,0,0};
                }
            }
        }
    }
}

void Chunk::CreateChunkMesh(Chunk** chunks){
    if(chunkMesh != nullptr){
        chunkMesh->indicesCount = 0;
        chunkMesh->vertices.clear();
        chunkMesh->indices.clear();
    }

    for(i32 x = 0; x < CHUNK_SIZE; x++){
        for(i32 y = 0; y < CHUNK_HEIGHT; y++){
            for(i32 z = 0; z < CHUNK_SIZE; z++){
                    CreateBlock(chunks, x,y,z);
                }
        }
    }

    chunkMesh->mesh.InitializeChunkMesh(GL_DYNAMIC_DRAW, GetChunkMeshData(), GetChunkMeshSize(), GetChunkIndicesData(), GetChunkIndicesSize());
}

void Chunk::AddFace(u32 faceIndex, glm::vec3 position){
    const glm::ivec3 pos = position;
    for(i32 i = 0; i < 4; i++){
        chunkMesh->vertices.push_back(blockModels->at(blocks->at
            (chunkData[pos.x][pos.y][pos.z].blockID).blockModelID)
            .vertices[(faceIndex*12) + 0 + (i*3)] + position.x);
        
        chunkMesh->vertices.push_back(blockModels->at(blocks->at
            (chunkData[pos.x][pos.y][pos.z].blockID).blockModelID)
            .vertices[(faceIndex*12) + 1 + (i*3)] + position.y);
        
        chunkMesh->vertices.push_back(blockModels->at(blocks->at
            (chunkData[pos.x][pos.y][pos.z].blockID).blockModelID)
            .vertices[(faceIndex*12) + 2 + (i*3)] + position.z);
    
        chunkMesh->vertices.push_back(i);
        chunkMesh->vertices.push_back(faceIndex);
        
        switch (faceIndex){
            case 0:
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).front.x);
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).front.y); 
                break;
            case 1:
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).back.x);
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).back.y); 
                break;
            case 2:
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).left.x);
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).left.y); 
                break;
            case 3:
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).right.x);
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).right.y); 
                break;
            case 4:
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).top.x);
                chunkMesh->vertices.push_back(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).top.y);   
                break;  
            case 5:

                break;  
        }
    }
    if(faceIndex % 2 == 0){
        for(u32 i : VerticesList::indicesCube[0]){
                chunkMesh->indices.push_back(i + (chunkMesh->indicesCount*4));
        }
    }else{
        for(u32 i : VerticesList::indicesCube[1]){
                chunkMesh->indices.push_back(i + (chunkMesh->indicesCount*4));
        }
    }
    chunkMesh->indicesCount++;
    
}


bool Chunk::ConstainsBlock(i32 x, i32 y, i32 z){
    return chunkData[x][y][z].blockID ? true : false;
}
bool Chunk::IsWithinChunk(i32 x, i32 y, i32 z){
    if(x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_SIZE){
        return true;
    }
    return false;
}

f32* Chunk::GetChunkMeshData(){
    return chunkMesh->vertices.data();
}

u32* Chunk::GetChunkIndicesData(){
    return chunkMesh->indices.data();
}

u32 Chunk::GetChunkMeshSize(){
    return chunkMesh->vertices.size();
}

u32 Chunk::GetChunkIndicesSize(){
    return chunkMesh->indices.size();
}

void Chunk::AddBlock(Chunk** chunks, i32 x, i32 y, i32 z, BlockInstanceData block){
    if(IsWithinChunk(x,y,z) && chunkData[x][y][z].blockID == 0){
        chunkData[x][y][z] = block;
        CreateChunkMesh(chunks);
        SendMeshData();
    }
}

void Chunk::RemoveBlock(Chunk** chunks, i32 x, i32 y, i32 z){
    if(IsWithinChunk(x,y,z) && chunkData[x][y][z].blockID > 0){
        chunkData[x][y][z] = {0,0,0,0};
        CreateChunkMesh(chunks);
        SendMeshData();
    }
}

void Chunk::Draw(glm::vec3 cameraPosition, glm::vec3 fogColor){
    if(chunkMesh != nullptr){
        chunkMesh->mesh.DrawChunk(GetChunkIndicesSize(), glm::vec3(position.x*CHUNK_SIZE, 0.0, position.y*CHUNK_SIZE), cameraPosition, fogColor);
    }
}


void Chunk::SendMeshData(){
    if(chunkMesh != nullptr){
        chunkMesh->mesh.SendChunkData(GetChunkMeshData(), GetChunkMeshSize(), GetChunkIndicesData(), GetChunkIndicesSize());  
    }
}

glm::vec2 Chunk::GetPosition(){
    return position;
}

i32 Chunk::GetCoordinateTerrainHeight(i32 x, i32 z){
    for(i32 y = CHUNK_HEIGHT-1; y > 0; y--){
        if(chunkData[x][y][z].blockID != 0){
            return y;
        }
    }

    return 0;
}


void Chunk::CreateBlock(Chunk** chunks, i32 x, i32 y, i32 z){
    if(chunkData[x][y][z].blockID != 0){
    if(z - 1 < 0){
        if(chunks[0] != nullptr){
            if(chunks[0]->chunkData[x][y][CHUNK_SIZE-1].blockID == 0){
                    AddFace(0, glm::vec3(x,y,z));
            }    
        }
        else{
            AddFace(0, glm::vec3(x,y,z));
        }
    }
    else if(chunkData[x][y][z-1].blockID == 0){
        AddFace(0, glm::vec3(x,y,z));
    }

    if(z + 1 >= CHUNK_SIZE){
        if(chunks[1] != nullptr){
            if(chunks[1]->chunkData[x][y][0].blockID == 0){
                AddFace(1, glm::vec3(x,y,z));
            }    
        }
        else{
            AddFace(1, glm::vec3(x,y,z));
        }
    }
    else if(chunkData[x][y][z+1].blockID == 0){
        AddFace(1, glm::vec3(x,y,z));
    }

    if(x - 1 < 0){
        if(chunks[2] != nullptr){
            if(chunks[2]->chunkData[CHUNK_SIZE-1][y][z].blockID == 0){
                AddFace(2, glm::vec3(x,y,z));
            }    
        }
        else{
            AddFace(2, glm::vec3(x,y,z));
        }
    }
    else if(chunkData[x-1][y][z].blockID == 0){
        AddFace(2, glm::vec3(x,y,z));
    }

    if(x + 1 >= CHUNK_SIZE){
        if(chunks[3] != nullptr){
            if(chunks[3]->chunkData[0][y][z].blockID == 0){
                AddFace(3, glm::vec3(x,y,z));
            }    
        }
        else{
            AddFace(3, glm::vec3(x,y,z));
        }
    }
        else if(chunkData[x+1][y][z].blockID == 0){
            AddFace(3, glm::vec3(x,y,z));
    }

    //Code for bottom face. Not used because player will never see them.
    // if(y - 1 < 0){
    //     AddFullBlockFace(5, blocks[chunkData[x][y][z]].bottom, glm::vec3(x,y,z));
    // }
    // else if(chunkData[x][y-1][z] == 0){
    //     AddFullBlockFace(5, blocks[chunkData[x][y][z]].bottom, glm::vec3(x,y,z));
    // }

    if(y + 1 >= CHUNK_HEIGHT){
        AddFace(4, glm::vec3(x,y,z));
    }
    else if(chunkData[x][y+1][z].blockID == 0){
        AddFace(4, glm::vec3(x,y,z));
    }    
    }
}

void Chunk::RequestAvailableChunkMesh(ChunkMesh* chunkMesh){
    if(chunkMesh != nullptr){
        this->chunkMesh = chunkMesh;
        hasChunkMesh = 1;
    }
}

void Chunk::DisposeChunkMesh(){
        chunkMesh = nullptr;
        hasChunkMesh = 0;
}

ChunkMesh* Chunk::GetChunkMesh(){
    return chunkMesh;
}

glm::vec3 Chunk::GetChunkCenter(){
    return chunkCenter;
}
