#include "chunk.h"
#include <iostream>

Chunk::Chunk(){
}

Chunk::~Chunk(){

}

void Chunk::CreateChunkDataOverworld(std::vector<fnl_state> noise, std::map<u8, BlockData>* blocks, std::map<std::string, BlockModelData>* blockModels, BiomeData biome, i32 x, i32 z){
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
                i32 tempValue = (fnlGetNoise3D(&noise[0], x + (position.x*CHUNK_SIZE), y, z + (position.y*CHUNK_SIZE))+1) + (y*0.02f);
                if(tempValue > 0){
                    if(y == value){
                        chunkData[x][y][z] = {biome.biomeLayersBlockID[0],0,0,0};
                    }
                    else if(y < value && y > value-3){
                        chunkData[x][y][z] = {biome.biomeLayersBlockID[1],0,0,0};
                    }
                    else if(y <= value-3 || y == 0){
                        chunkData[x][y][z] = {biome.biomeLayersBlockID[2],0,0,0};
                    }
                    else{
                        chunkData[x][y][z] = {0,0,0,0};
                    }
                }
                else{
                    if(y == 0){
                        chunkData[x][y][z] = {biome.biomeLayersBlockID[2],0,0,0};
                    }else{
                        chunkData[x][y][z] = {0,0,0,0};
                    }
                }
            }
        }
    }
}


void Chunk::CreateChunkDataDungeon(Dungeon* dungeon, std::map<u8, BlockData>* blocks, std::map<std::string, BlockModelData>* blockModels, BiomeData biome, i32 x, i32 z){
    this->blocks = blocks;
    this->blockModels = blockModels;
    position = glm::vec2(x,z);
    chunkCenter = glm::vec3((position.x+0.5)*CHUNK_SIZE,
        CHUNK_HEIGHT*0.5,
        (position.y+0.5)*CHUNK_SIZE);

    for(i32 x = 0; x < CHUNK_SIZE; x++){
        for(i32 z = 0; z < CHUNK_SIZE; z++){
            DungeonMap dungeonValue = dungeon->GetDungeonMap()[int((z + (position.y*CHUNK_SIZE)) + (x + (position.x*CHUNK_SIZE)) * 64)];
            for(i32 i = 0; i < 1; i++){
                for(i32 y = i*4; y < i*4+4; y++){  
                    if(y <= dungeonValue.elevation + i*4 && y > i*4){
                        chunkData[x][y][z] = {dungeonValue.blockType,0,0,0};
                    }
                    else if(y == i*4){
                        chunkData[x][y][z] = {dungeonValue.blockType,0,0,0};
                    }
                    else{
                        chunkData[x][y][z] = {0,0,0,0};
                    }
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

    chunkMesh->mesh.InitializeMesh(GL_DYNAMIC_DRAW, GetChunkMeshData(), GetChunkMeshSize(), sizeof(ChunkVertex), GetChunkIndicesData(), GetChunkIndicesSize());
    chunkMesh->mesh.AddIntAttribute(GL_UNSIGNED_INT, sizeof(u32), 1, 2, 0);
    chunkMesh->mesh.AddAttribute(GL_FLOAT, sizeof(f32), 1, 2, 1);
}

void Chunk::OrganizeChunk(Chunk** chunks){
    for(i32 x = 0; x < CHUNK_SIZE; x++){
        for(i32 y = 0; y < CHUNK_HEIGHT; y++){
            for(i32 z = 0; z < CHUNK_SIZE; z++){
                    OrganizeBlock(chunks, x,y,z);
                }
        }
    }
}

void Chunk::AddFace(u32 faceIndex, glm::ivec3 position){
    const glm::ivec3 pos = position;
    for(i32 i = 0; i < 4; i++){
        ChunkVertex vertex;
        vertex.packedPositionAndUv = 0;

        vertex.packedPositionAndUv = 
            (VerticesList::verticesIntCube[(faceIndex*12) + 0 + (i*3)] + position.x) |
            (VerticesList::verticesIntCube[(faceIndex*12) + 1 + (i*3)] + position.y) << 6 |
            (VerticesList::verticesIntCube[(faceIndex*12) + 2 + (i*3)] + position.z) << 12;

        switch(faceIndex){
            case 0:
                vertex.packedPositionAndUv |=
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).front.x) << 18 |
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).front.y) << 25; 
                break;
            case 1:
                vertex.packedPositionAndUv |=
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).back.x)<< 18 |
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).back.y)<< 25; 
                break;
            case 2:
                vertex.packedPositionAndUv |=
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).left.x)<< 18 |
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).left.y)<< 25; 
                break;
            case 3:
                vertex.packedPositionAndUv |=
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).right.x) << 18 |
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).right.y) << 25;  
                break;
            case 4:
                vertex.packedPositionAndUv |=
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).top.x) << 18 |
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).top.y) << 25; 
                break;  
            case 5:
                vertex.packedPositionAndUv |=
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).bottom.x) << 18 |
                    u32(blocks->at(chunkData[pos.x][pos.y][pos.z].blockID).bottom.y) << 25;   
                break;  
        }

        vertex.face = faceIndex;
        chunkMesh->vertices.push_back(vertex);
    }
    
    if(faceIndex % 2 == 0){
        for(u32 i : VerticesList::indicesCube[0]){
                chunkMesh->indices.push_back(i + (chunkMesh->indicesCount));
        }
    }else{
        for(u32 i : VerticesList::indicesCube[1]){
                chunkMesh->indices.push_back(i + (chunkMesh->indicesCount));
        }
    }
    chunkMesh->indicesCount+=4;
}


bool Chunk::ConstainsBlock(i32 x, i32 y, i32 z){
    return chunkData[x][y][z].blockID ? 1 : 0;
}
bool Chunk::IsWithinChunk(i32 x, i32 y, i32 z){
    if(x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_SIZE){
        return true;
    }
    return false;
}

ChunkVertex* Chunk::GetChunkMeshData(){
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
        glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(position.x*CHUNK_SIZE, 0.0, position.y*CHUNK_SIZE));
        glUniformMatrix4fv(0,1, false, glm::value_ptr(transform));
        glUniform3fv(3, 1, glm::value_ptr(cameraPosition));
        glUniform3fv(4, 1, glm::value_ptr(fogColor));
        chunkMesh->mesh.DrawMesh(GetChunkIndicesSize());
    }
}


void Chunk::SendMeshData(){
    if(chunkMesh != nullptr){
        chunkMesh->mesh.SendData(GetChunkMeshData(), GetChunkMeshSize(), sizeof(ChunkVertex), GetChunkIndicesData(), GetChunkIndicesSize());  
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
                        AddFace(0, glm::ivec3(x,y,z));
                }    
            }
            else{
                AddFace(0, glm::ivec3(x,y,z));
            }
        }
        else if(chunkData[x][y][z-1].blockID == 0){
            AddFace(0, glm::ivec3(x,y,z));
        }

        if(z + 1 >= CHUNK_SIZE){
            if(chunks[1] != nullptr){
                if(chunks[1]->chunkData[x][y][0].blockID == 0){
                    AddFace(1, glm::ivec3(x,y,z));
                }    
            }
            else{
                AddFace(1, glm::ivec3(x,y,z));
            }
        }
        else if(chunkData[x][y][z+1].blockID == 0){
            AddFace(1, glm::ivec3(x,y,z));
        }

        if(x - 1 < 0){
            if(chunks[2] != nullptr){
                if(chunks[2]->chunkData[CHUNK_SIZE-1][y][z].blockID == 0){
                    AddFace(2, glm::ivec3(x,y,z));
                }    
            }
            else{
                AddFace(2, glm::ivec3(x,y,z));
            }
        }
        else if(chunkData[x-1][y][z].blockID == 0){
            AddFace(2, glm::ivec3(x,y,z));
        }

        if(x + 1 >= CHUNK_SIZE){
            if(chunks[3] != nullptr){
                if(chunks[3]->chunkData[0][y][z].blockID == 0){
                    AddFace(3, glm::ivec3(x,y,z));
                }    
            }
            else{
                AddFace(3, glm::ivec3(x,y,z));
            }
        }
            else if(chunkData[x+1][y][z].blockID == 0){
                AddFace(3, glm::ivec3(x,y,z));
        }

        if(y - 1 < 0){
            AddFace(5, glm::ivec3(x,y,z));
        }
        else if(chunkData[x][y-1][z].blockID == 0){
            AddFace(5, glm::ivec3(x,y,z));
        }

        // if(y == 0 && chunkData[x][y+1][z].blockID == 0){
        //     AddFace(5, glm::vec3(x,y,z));
        // }

        if(y + 1 >= CHUNK_HEIGHT){
            AddFace(4, glm::ivec3(x,y,z));
        }
        else if(chunkData[x][y+1][z].blockID == 0){
            AddFace(4, glm::ivec3(x,y,z));
        }    
    }
}

void Chunk::OrganizeBlock(Chunk** chunks, i32 x, i32 y, i32 z){
    if(chunkData[x][y][z].blockID != 0){
        if(z - 1 < 0){
            if(chunks[0] != nullptr){
                if(chunks[0]->chunkData[x][y][CHUNK_SIZE-1].blockID == 0){
                    chunkData[x][y][z].visibility = 1;
                    return;
                }    
            }
        }
        else if(chunkData[x][y][z-1].blockID == 0){
            chunkData[x][y][z].visibility = 1;
            return;
        }   
        if(z + 1 >= CHUNK_SIZE){
            if(chunks[1] != nullptr){
                if(chunks[1]->chunkData[x][y][0].blockID == 0){
                    chunkData[x][y][z].visibility = 1;
                    return;
                }    
            }
        }
        else if(chunkData[x][y][z+1].blockID == 0){
            chunkData[x][y][z].visibility = 1;
            return;
        }   
        if(x - 1 < 0){
            if(chunks[2] != nullptr){
                if(chunks[2]->chunkData[CHUNK_SIZE-1][y][z].blockID == 0){
                    chunkData[x][y][z].visibility = 1;
                    return;
                }    
            }

        }
        else if(chunkData[x-1][y][z].blockID == 0){
            chunkData[x][y][z].visibility = 1;
            return;
        }   
        if(x + 1 >= CHUNK_SIZE){
            if(chunks[3] != nullptr){
                if(chunks[3]->chunkData[0][y][z].blockID == 0){
                    chunkData[x][y][z].visibility = 1;
                    return;
                }    
            }
        }
        else if(chunkData[x+1][y][z].blockID == 0){
            chunkData[x][y][z].visibility = 1;
            return;
        }  

        if(y - 1 < 0){
            
        }
        else if(chunkData[x][y-1][z].blockID == 0){
            chunkData[x][y][z].visibility = 1;
            return;
        }

        if(y + 1 > CHUNK_HEIGHT){
            chunkData[x][y][z].visibility = 1;
            return;
        }
        else if(chunkData[x][y+1][z].blockID == 0){
            chunkData[x][y][z].visibility = 1;
            return;
        }

        chunkData[x][y][z].visibility = 0;            
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
