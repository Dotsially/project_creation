#include "mask.h"
#include "math.h"

void Mask::InitializeMask(u32 width, u32 height){
    this->width = width;
    this->height = height;
    maskData = (f32*)calloc(width*height, sizeof(f32));
}

void Mask::DestroyMask(){
    free(maskData);
}

void Mask::CreateSphereMask(glm::vec2 position, u32 radius){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            f32 distance = glm::distance(position, glm::vec2{x,y});
            maskData[y + x * height] = (f32)(glm::smoothstep((f32)radius, radius+30.0f, distance));
        }
    }
}

f32* Mask::GetMaskData(){
    return maskData;    
}