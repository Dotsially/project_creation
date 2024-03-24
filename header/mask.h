#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"


class Mask{
private:
    f32* maskData;
    u32 width;
    u32 height;
public:
    void InitializeMask(u32 width, u32 height);
    void DestroyMask();

    void CreateSphereMask(glm::vec2 position, u32 radius);
    f32* GetMaskData();
};