#include "overworld_mesh.h"
#include <iostream>

void OverworldMesh::InitializeMesh(TexturePacker* overworldPacker, TileData* worldTiles, i32 width, i32 height){
    for(i32 x = 1; x < width; x++){
        for(i32 y = 1; y < height; y++){
            for(i32 i = 0; i < 4; i++){
                vertices.push_back(VerticesList::verticesItem1x1[i*2]+x-1);
                vertices.push_back(VerticesList::verticesItem1x1[1+i*2]+y-1);
                vertices.push_back(1);

                i32 textureValue = CalculateBitTile(worldTiles, width, height, x, y);
                if(textureValue == 0){
                    glm::vec2 landIndex = overworldPacker->GetTextures()->at("land.png");
                    vertices.push_back(landIndex.x);
                    vertices.push_back(landIndex.y); 
                }
                else if(textureValue == 15){
                    glm::vec2 oceanIndex =  overworldPacker->GetTextures()->at("ocean.png"); 
                    vertices.push_back(oceanIndex.x);
                    vertices.push_back(oceanIndex.y);
                }
                else{
                    std::string texture = "ocean_" + std::to_string(textureValue) + ".png";
                    glm::vec2 oceanIndex =  overworldPacker->GetTextures()->at(texture); 
                    vertices.push_back(oceanIndex.x);
                    vertices.push_back(oceanIndex.y);
                }
            }
            
            for(u32 i : VerticesList::indicesEntity){
                indices.push_back(i + indicesCount); 
            }

            indicesCount += 4;
        }
    }

    mesh.InitializeMesh(GL_DYNAMIC_DRAW, vertices.data(), vertices.size(), indices.data(), indices.size());
    mesh.AddAttribute(3, 5, 0);
    mesh.AddAttribute(2, 5, 3);
}



void OverworldMesh::Draw(){
    mesh.DrawMesh(indices.size(), glm::mat4{});
}


i32 OverworldMesh::CalculateBitTile(TileData* worldTiles, i32 width, i32 height, i32 x, i32 y){
    i32 bitValue = 0;
    int count = 0;
    for(int i = y-1; i < y+1; i++){
        for(int j = x-1; j < x+1; j++){
            if(worldTiles[i + j * width].tileHeight == 1){
                bitValue+= glm::pow(2, count);
            }
            count++;
        }
    }

    return bitValue;
}