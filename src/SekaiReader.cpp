#include "sekai_reader.h"
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void SekaiReader::ReadBlocks(std::map<std::string, u8>* blockNames, std::map<u8,BlockData>* blocks, std::map<std::string, glm::vec2>* textures){
    std::vector<std::string> files = ReadDirectory("resources/data/blocks");
    i32 fileIDCount = 1;

    for(std::string file : files){
        BlockData block = ReadBlockFile(textures, "resources/data/blocks/" + file, fileIDCount);
        blockNames->operator[](block.blockNameID) = block.blockID;
        blocks->operator[](block.blockID) = block;
        fileIDCount++;
    }

}

BiomeData SekaiReader::ReadBiomeFile(std::string path){
    BiomeData biome;
    std::fstream fileStream(path);
    json jsonData = json::parse(fileStream);

    biome.name = jsonData["biome_name"];
    
    if(jsonData["biome_type"] == "overworld"){
        biome.type = 0;
    }
    else if(jsonData["biome_type"] == "dungeon"){
        biome.type = 1;
    }
    else{
        biome.type = 0;
    }

    biome.height = jsonData["biome_height"];
    biome.elevation = jsonData["biome_elevation"];

    for(int i = 0; i < 3; i++){
        biome.layers[i] = jsonData["biome_layers"][i];
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
            biome.noise.push_back(noiseData);
        }
    }

    fileStream.close();
    return biome;
}

void SekaiReader::ReadBiomes(std::map<std::string, BiomeData>* biomes){
    std::vector<std::string> files = ReadDirectory("resources/data/biomes");

    for(std::string file : files){
        BiomeData biome = ReadBiomeFile("resources/data/biomes/" + file);
        biomes->operator[](biome.name) = biome;
    }

}

BlockData SekaiReader::ReadBlockFile(std::map<std::string, glm::vec2>* textures, std::string path, u8 id){
    BlockData block;
    std::fstream fileStream(path);

    json jsonData = json::parse(fileStream);
    
    block.blockNameID = jsonData["block_name"];

    block.blockID = id;
    block.blockModelID = jsonData["block_model"];
    
    block.top = textures->at(jsonData["texture_top"]);
    block.bottom = textures->at(jsonData["texture_bottom"]);
    block.left = textures->at(jsonData["texture_left"]);
    block.right = textures->at(jsonData["texture_right"]);
    block.front = textures->at(jsonData["texture_front"]);
    block.back = textures->at(jsonData["texture_back"]);

    

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
    std::vector<std::string> files = ReadDirectory("resources/data/block_models");

    for(std::string file : files){
        BlockModelData model = ReadBlockModelFile("resources/data/block_models/" + file);
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

void SekaiReader::ReadItems(std::map<u8, ItemData>* items, std::map<std::string, glm::vec2>* textures){
    std::vector<std::string> files = ReadDirectory("resources/data/items");
    u8 fileCount = 0;
    for(std::string file : files){
        ItemData item = ReadItemFiles(textures, "resources/data/items/" + file);
        items->operator[](fileCount) = item;
        fileCount++;
    }
}

ItemData SekaiReader::ReadItemFiles(std::map<std::string, glm::vec2>* textures, std::string path){ 
    ItemData item;
    std::fstream fileStream(path);
    json jsonData = json::parse(fileStream);

    item.name = jsonData["name"];
    item.texture = textures->at(jsonData["texture"]);
    item.consumable = (i32)jsonData["consumable"];

    fileStream.close();
    return item;
}

void SekaiReader::ReadTextures(AtlasArray* atlasArray, std::map<std::string, glm::vec2>* textures, std::string path){
    std::vector<std::string> files = ReadDirectory(path);
    
    i32 width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    for(std::string file : files){
        std::string filePath = path + "/" + file;
        u8* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glm::vec2 textureCoordinates = glm::vec2(0,0);
            atlasArray->InsertTexture(data, &textureCoordinates, width, height);
            textures->operator[](file) = textureCoordinates;
        }
        stbi_image_free(data);
    }    

}