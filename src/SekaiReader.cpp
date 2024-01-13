#include "sekai_reader.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

std::vector<std::string> SekaiReader::ReadDirectory(std::string path){
    std::vector<std::string> files;

    DIR *directory;
    struct dirent *entry;
    if ((directory = opendir (path.c_str())) != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            char* fileName = entry->d_name;
            int nameLength = entry->d_namlen;
            std::string file = "";
            
            if(nameLength > 5){
                for(int i = 0; i < nameLength; i++){
                    file += fileName[i];
                }
                std::cout << file << std::endl;
                files.push_back(file);
            }
        }

        closedir (directory);
    } else {
        /* could not open directory */
        perror ("could not open directory");

    }


    return files;
}

void SekaiReader::ReadBlocks(std::map<std::string, u8>* blockNames, std::map<u8,BlockData>* blocks){
    std::vector<std::string> files = ReadDirectory("resources/blocks");
    i32 fileIDCount = 1;

    for(std::string file : files){
        BlockData block = ReadBlockFile("resources/blocks/" + file, fileIDCount);
        blockNames->operator[](block.blockNameID) = block.blockID;
        blocks->operator[](block.blockID) = block;
        fileIDCount++;
    }

}

BiomeData SekaiReader::ReadBiomeFile(std::string path){
    BiomeData biome;
    std::fstream fileStream(path);
    json jsonData = json::parse(fileStream);

    biome.biomeName = jsonData["biome_name"];
    biome.biomeType = jsonData["biome_type"];
    biome.height = jsonData["biome_height"];
    biome.elevation = jsonData["biome_elevation"];

    for(int i = 0; i < 3; i++){
        biome.biomeLayers[i] = jsonData["biome_layers"][i];
    }

    if(jsonData.contains("biome_fog")){
        for(auto it = jsonData["biome_fog"].begin(); 
            it != jsonData["biome_fog"].end(); ++it){   
            BiomeFogData biomeFogData; 
            for(int i = 0; i < 3; i++){
                biomeFogData.fogColor[i] = it.value()["fog_color"][i];
                biomeFogData.skyColor[i] = it.value()["sky_color"][i];
            }
            biome.colors.operator[](it.key()) = biomeFogData;   
        }
    }

    if(jsonData.contains("biome_noise")){
        for(auto it : jsonData["biome_noise"]){
            BiomeNoiseData noiseData;
            noiseData.frequency = it["frequency"];
            noiseData.octaves = it["octaves"];
            noiseData.lacuranity = it["lacunarity"];
            noiseData.gain = it["gain"];
            noiseData.weightedStrength = it["weighted_strength"];
            noiseData.noiseType = it["noise_type"];
            noiseData.fractalType = it["noise_fractal_type"];
            noiseData.cellularReturnType = it["noise_cellular_return_type"];
            if(it.contains("divisor")){
                if(it["divisor"] > 0){
                    noiseData.divisor = it["divisor"];
                }
            }
            biome.biomeNoise.push_back(noiseData);
        }
    }

    fileStream.close();
    return biome;
}

void SekaiReader::ReadBiomes(std::map<std::string, BiomeData>* biomes){
    std::vector<std::string> files = ReadDirectory("resources/biomes");

    for(std::string file : files){
        BiomeData biome = ReadBiomeFile("resources/biomes/" + file);
        biomes->operator[](biome.biomeName) = biome;
    }

}

BlockData SekaiReader::ReadBlockFile(std::string path, u8 id){
    BlockData block;
    std::fstream fileStream(path);

    json jsonData = json::parse(fileStream);
    
    block.blockNameID = jsonData["block_name"];

    block.blockID = id;
    block.blockModelID = jsonData["block_model"];
    
    block.top = glm::vec2(jsonData["uv_top"][0], jsonData["uv_top"][1]);
    block.bottom = glm::vec2(jsonData["uv_bottom"][0], jsonData["uv_bottom"][1]);
    block.left = glm::vec2(jsonData["uv_left"][0], jsonData["uv_left"][1]);
    block.right = glm::vec2(jsonData["uv_right"][0], jsonData["uv_right"][1]);
    block.front = glm::vec2(jsonData["uv_front"][0], jsonData["uv_front"][1]);
    block.back = glm::vec2(jsonData["uv_back"][0], jsonData["uv_back"][1]);

    

    fileStream.close();

    return block;
}


BlockModelData SekaiReader::ReadBlockModelFile(std::string path){
    BlockModelData model;
    std::fstream fileStream(path);

    json jsonData = json::parse(fileStream);
    
    model.modelName = jsonData["name"];
    model.modelType = jsonData["type"];

    for(int i = 0 ; i < 3; i++){
        model.from[i] = jsonData["from"][i];
        model.to[i] = jsonData["to"][i];
    }

    fileStream.close();
    return model;
}


void SekaiReader::ReadBlockModels(std::map<std::string, BlockModelData>* blockModels){
    std::vector<std::string> files = ReadDirectory("resources/block_models");

    for(std::string file : files){
        BlockModelData model = ReadBlockModelFile("resources/block_models/" + file);
        blockModels->operator[](model.modelName) = model;
    }
}

void SekaiReader::ReadWorld(WorldData* worldData){
    std::fstream fileStream("resources/world.json");
    json jsonData = json::parse(fileStream);

    worldData->regionSize = jsonData["region_size"];
    worldData->biome = jsonData["biome"];
    worldData->renderDistance = jsonData["render_distance"];

    fileStream.close();
}