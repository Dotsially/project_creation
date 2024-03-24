#include "dynamic_array.h"
#include <iostream>
AtlasArray::AtlasArray(){
    array.array = (u8*)calloc(initialSize, sizeof(u8));
    array.size = initialSize;
    array.lastHeight = 0;
    array.lastWidth = 0;
    array.usedHeight = 0;
    array.usedWidth  = 0;
    initialized = 1;   
}

AtlasArray::~AtlasArray(){
    FreeArray();    
}

void AtlasArray::InsertTexture(u8* texture, glm::vec2* textureCoordinates, u32 width, u32 height){
    if(array.lastWidth-1 + width < 128){
        textureCoordinates->x = array.lastWidth/16;
        textureCoordinates->y = array.lastHeight/16;
        for(int x = array.lastHeight; x < (array.lastHeight+height); x++){
            for(int y = array.lastWidth; y < (array.lastWidth+width); y++){
                for(int i = 0; i < 4; i++){
                    array.array[(y*4)+((x*4)*128)+i] = 
                        texture[((y-array.lastWidth)*4)+(((x-array.lastHeight)*4)*width)+i];       
                }
            }
        }

        array.lastWidth += width;
    }
    else if(array.lastHeight-1 + height < 128){
        array.lastHeight += height;
        array.lastWidth = 0;
        
        textureCoordinates->x = array.lastWidth/16;
        textureCoordinates->y = array.lastHeight/16;

        for(int x = array.lastHeight; x < (array.lastHeight+height); x++){
            for(int y = array.lastWidth; y < (array.lastWidth+width); y++){
                for(int i = 0; i < 4; i++){
                    array.array[(y*4)+((x*4)*128)+i] = 
                        texture[((y-array.lastWidth)*4)+(((x-array.lastHeight)*4)*width)+i];       
                }
            }
        }

        array.lastWidth += width;
    }
}

void AtlasArray::FreeArray(){
    if(initialized){
        free(array.array);
        array.array = nullptr;
        array.size = 0;
        array.lastHeight = 0;
        array.lastWidth = 0;
        initialized = 0;
    }
}


u32 AtlasArray::GetSize(){
    return 128;
}

u8* AtlasArray::GetAtlasArray(){
    return array.array;
}