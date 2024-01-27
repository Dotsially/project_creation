#include "biome.h"
#include <iostream>

Biome::Biome(BlockManager* blockManager){
    sekaiReader.ReadBiomes(&biomes);
    
    for (auto& [key, biome] : biomes){ 
        for(int i = 0; i < 3; i++){
            biome.biomeLayersBlockID[i] = 
                blockManager->BlockNameToBlockId(biome.layers[i]);
        }
    }
}

Biome::~Biome(){


}

BiomeData Biome::GetBiome(std::string biome){
    return biomes[biome];
}