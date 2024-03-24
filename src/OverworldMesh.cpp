#include "overworld_mesh.h"
#include <iostream>

void OverworldMesh::InitializeMesh(TexturePacker* overworldPacker, TileData* worldTiles, i32 width, i32 height, i32 screenWidth, i32 screenHeight){
    vertices.clear();
    indices.clear();

    for(i32 x = 1; x < width; x++){
        for(i32 y = 1; y < height; y++){
            GenerateMesh(overworldPacker, worldTiles, width, height, x, y);
        }
    }

    mesh.InitializeMesh(GL_DYNAMIC_DRAW, vertices.data(), vertices.size(), indices.data(), indices.size());
    mesh.AddAttribute(3, 6, 0);
    mesh.AddAttribute(2, 6, 3);
    mesh.AddAttribute(1, 6, 5);

    for(i32 i = 0; i < 4; i++){
        waterVertices[i*3] = VerticesList::verticesItem1x1[i*2] * width;
        waterVertices[1+i*3] = VerticesList::verticesItem1x1[1+i*2] * height;
        waterVertices[2+i*3] = 0;
    }

    int count = 0;
    for(u32 i : VerticesList::indicesEntity){
        waterIndices[count] = i;
        count++; 
    }

    waterMesh.InitializeMesh(GL_DYNAMIC_DRAW, waterVertices, 12, waterIndices, 6);
    waterMesh.AddAttribute(3, 3, 0);
}

void OverworldMesh::ReloadMesh(TexturePacker* overworldPacker, TileData* worldTiles, i32 width, i32 height){
    vertices.clear();
    indices.clear();
    indicesCount = 0;

    for(i32 x = 0; x < width; x++){
        for(i32 y = 0; y < height; y++){
            GenerateMesh(overworldPacker, worldTiles, width, height, x, y);
        }
    }
    
    mesh.SendData(vertices.data(), vertices.size(), indices.data(), indices.size());
}



void OverworldMesh::Draw(){
    glUniform3fv(2,6, glm::value_ptr(colors[0]));
    mesh.DrawMesh(indices.size());
}

void OverworldMesh::DrawWater(){
    waterMesh.DrawMesh(6);
}


i32 OverworldMesh::MarchingSquares(TileData* worldTiles, i32 width, i32 height, i32 x, i32 y){
    i32 bitValue = 0;
    i32 count = 0;
    for(i32 i = y-1; i <= y; i++){
        for(i32 j = x-1; j <= x; j++){
            if(worldTiles[i + j * height].tileHeight < 5){
                bitValue+= glm::pow(2, count);
            }
            count++;
        }
    }

    return bitValue;
}

void OverworldMesh::GenerateMesh(TexturePacker* overworldPacker, TileData* worldTiles, i32 width, i32 height, i32 x, i32 y){
        for(i32 i = 0; i < 4; i++){
            //glm::vec4 quadrantValue = Subblob(worldTiles, width, height, x, y);
        
            if(worldTiles[y + x*height].tileType < 3){
                vertices.push_back(VerticesList::verticesItem1x1[i*2]+x);
                vertices.push_back(VerticesList::verticesItem1x1[1+i*2]+y);
                vertices.push_back(1);
                glm::vec2 oceanIndex = overworldPacker->GetTextures()->at("land.png"); 
                vertices.push_back(oceanIndex.x);
                vertices.push_back(oceanIndex.y);
                //std::string texture = "ocean_" + std::to_string((int)quadrantValue[0]) + "_" + std::to_string(0) + ".png";
                //std::cout << texture << std::endl;
                // oceanIndex =  overworldPacker->GetTextures()->at(texture); 
                // vertices.push_back(oceanIndex.x);
                // vertices.push_back(oceanIndex.y);             
    
                vertices.push_back(worldTiles[y + x*height].tileType);
            } 
        }
        for(u32 i : VerticesList::indicesEntity){
            indices.push_back(i + indicesCount); 
        }
        indicesCount += 4;
}

glm::vec4 OverworldMesh::Subblob(TileData* worldTiles, i32 width, i32 height, i32 x, i32 y){
    //2 == current tile
    u8 neighbors[9] ={0,0,0,0,2,0,0,0,0};
    glm::vec4 quadrantValues = glm::vec4{0};

    for(i32 i = y-1; i <= y+1; i++){
        for(i32 j = x-1; j <= x+1; j++){
            if(i > 0 && i < height && j > 0 && j < width){
                if(worldTiles[i + j * height].tileType == 3){
                    neighbors[i + j * 3] = 1;
                }
            }
        }
    }  

    quadrantValues[0] = neighbors[3] + neighbors[1]*2;
         if(neighbors[3] && neighbors[0] && neighbors[1]) quadrantValues[0] = 4;

    quadrantValues[1] = neighbors[6] + neighbors[1]*2;
    if(neighbors[6] && neighbors[2] && neighbors[1]) quadrantValues[1] = 4;

    quadrantValues[2] = neighbors[3] + neighbors[8]*2;
    if(neighbors[3] && neighbors[7] && neighbors[8]) quadrantValues[2] = 4;

    quadrantValues[3] = neighbors[6] + neighbors[8]*2;
    if(neighbors[6] && neighbors[7] && neighbors[9]) quadrantValues[3] = 4;

    return quadrantValues;
}