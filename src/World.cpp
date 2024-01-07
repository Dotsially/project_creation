#include "world.h"
#include <iostream>

World::World(std::map<u8, BlockData>* blocks, std::map<std::string, BlockModelData>* blockModels, Biome biomes){
    sekaiReader.ReadWorld(&worldData);
    REGION_SIZE = worldData.regionSize;
    
    chunks = new Chunk[REGION_SIZE*REGION_SIZE];
    biome = biomes.GetBiome(worldData.biome);
    
    
    std::vector<fnl_state> biomeNoise;

    for(int i = 0; i < biome.biomeNoise.size(); i++){
        fnl_state noise = fnlCreateState();
        noise.noise_type = fnl_noise_type(biome.biomeNoise[i].noiseType);
        noise.fractal_type = fnl_fractal_type(biome.biomeNoise[i].fractalType);
        noise.cellular_return_type = fnl_cellular_return_type(biome.biomeNoise[i].cellularReturnType);
        noise.frequency = biome.biomeNoise[i].frequency;
        noise.octaves = biome.biomeNoise[i].octaves;
        noise.lacunarity = biome.biomeNoise[i].lacuranity;
        noise.gain = biome.biomeNoise[i].gain;
        noise.weighted_strength = biome.biomeNoise[i].weightedStrength;
        biomeNoise.push_back(noise);
    }

    for(i32 x = 0; x < REGION_SIZE; x++){
        for(i32 z = 0; z < REGION_SIZE; z++){
            chunks[z+x*REGION_SIZE].CreateChunkData(blocks, blockModels, biome, biomeNoise, x,z);
        }
    }

    Chunk* neighbors[4];
    for(i32 x = 0; x < REGION_SIZE; x++){
        for(i32 z = 0; z < REGION_SIZE; z++){
            GetNeighbors(neighbors,x,z);
            chunks[z+x*REGION_SIZE].CreateChunkMesh(neighbors);
        }
    }
}

World::~World(){
    delete(chunks);
}

void World::Draw(glm::vec3 playerPos, glm::vec3 cameraPosition){
    
    glm::vec3 fogColor = glm::vec3{biome.colors["clear"].fogColor[0], 
        biome.colors["clear"].fogColor[1],
        biome.colors["clear"].fogColor[2]};
    i32 renderDistance = 4;
    glm::ivec2 playerChunkPos = glm::vec2(playerPos.x/32, playerPos.z/32);
    for(i32 x = 0; x < REGION_SIZE; x++){
        for(i32 z = 0; z < REGION_SIZE; z++){
            glm::ivec2 chunkPos = chunks[z+x*REGION_SIZE].GetPosition();
            if(playerChunkPos.x-chunkPos.x <= renderDistance 
                && playerChunkPos.x-chunkPos.x >= -renderDistance 
                && playerChunkPos.y-chunkPos.y <= renderDistance 
                && playerChunkPos.y-chunkPos.y >= -renderDistance){
                chunks[z+x*REGION_SIZE].Draw(cameraPosition, fogColor);
            }
        }
    }
}


u8 World::IsWithinWorld(i32 x, i32 y, i32 z){
    if(x >= 0 && x < CHUNK_SIZE * REGION_SIZE 
    && y >= 0 && y < CHUNK_HEIGHT 
    && z >= 0 && z < CHUNK_SIZE * REGION_SIZE){
        return true;
    }
    return false;
}

u8 World::IsWithinWorld(glm::vec3 position){
    return IsWithinWorld(position.x, position.y, position.z);
}

u8 World::ContainsBlock(i32 x, i32 y, i32 z){
    glm::ivec2 chunkCoordinate(x/32, z/32);
    return chunks[chunkCoordinate.y + chunkCoordinate.x*REGION_SIZE]
        .ConstainsBlock(x%32, y, z%32);
}

void World::AddBlock(i32 x, i32 y, i32 z, BlockInstanceData block){
    if(IsWithinWorld(x,y,z)){
        glm::ivec2 chunkCoordinate(x/32, z/32);
        Chunk* neighbors[4];
        GetNeighbors(neighbors, chunkCoordinate.x, chunkCoordinate.y);
        chunks[chunkCoordinate.y + chunkCoordinate.x*REGION_SIZE]
            .AddBlock(neighbors, x%32, y, z%32, block);
    }
}

void World::RemoveBlock(i32 x, i32 y, i32 z){
    glm::ivec2 chunkCoordinate(x/32, z/32);
    Chunk* neighbors[4];
    GetNeighbors(neighbors, chunkCoordinate.x, chunkCoordinate.y);
    chunks[chunkCoordinate.y + chunkCoordinate.x*REGION_SIZE]
        .RemoveBlock(neighbors, x%32, y, z%32);

    Chunk* tempNeighbors[4];
    if(z%32 == 0 && neighbors[0] != nullptr){
        GetNeighbors(tempNeighbors, chunkCoordinate.x, chunkCoordinate.y-1);
        neighbors[0]->CreateChunkMesh(tempNeighbors);
        neighbors[0]->SendMeshData();
    }
    if(z%32 == 31 && neighbors[1] != nullptr){
        GetNeighbors(tempNeighbors, chunkCoordinate.x, chunkCoordinate.y+1);
        neighbors[1]->CreateChunkMesh(tempNeighbors);
        neighbors[1]->SendMeshData();
    }
    if(x%32 == 0 && neighbors[2] != nullptr){
        GetNeighbors(tempNeighbors, chunkCoordinate.x-1, chunkCoordinate.y);
        neighbors[2]->CreateChunkMesh(tempNeighbors);
        neighbors[2]->SendMeshData();
    }
    if(x%32 == 31 && neighbors[3] != nullptr){
        GetNeighbors(tempNeighbors, chunkCoordinate.x+1, chunkCoordinate.y);
        neighbors[3]->CreateChunkMesh(tempNeighbors);
        neighbors[3]->SendMeshData();
    }

}


void World::GetNeighbors(Chunk* neighbors[4], i32 x, i32 z){
        if(z - 1 >= 0){
            neighbors[0] = &chunks[(z-1)+x*REGION_SIZE];
        }else
        {
            neighbors[0] = nullptr;
        }
        if((z + 1) < REGION_SIZE){
            neighbors[1] = &chunks[(z+1)+x*REGION_SIZE];
        }else
        {
            neighbors[1] = nullptr;
        }
        if(x - 1 >= 0){
            neighbors[2] = &chunks[z+(x-1)*REGION_SIZE];
        }else
        {
            neighbors[2] = nullptr;
        }
        if((x + 1) < REGION_SIZE){
            neighbors[3] = &chunks[z+(x+1)*REGION_SIZE];
        }else
        {
            neighbors[3] = nullptr;
        }
}

i32 World::GetTerrainHeight(i32 x, i32 z){
    glm::ivec2 chunkCoordinate(x/32, z/32);
    return chunks[chunkCoordinate.y + chunkCoordinate.x*REGION_SIZE]
        .GetCoordinateTerrainHeight(x%32,z%32);
}

glm::vec3 World::GetSkyColor(){
    glm::vec3 skyColor = glm::vec3{biome.colors["clear"].skyColor[0],
        biome.colors["clear"].skyColor[1],
        biome.colors["clear"].skyColor[2]}; 
    
    return skyColor;
}