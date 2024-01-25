#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"
#include <stdlib.h>

typedef struct Array{
  u8* array;
  u32 usedWidth;
  u32 usedHeight;
  u32 lastWidth;
  u32 lastHeight;
  u32 size;
} Array;

class AtlasArray{
private:
    Array array;
    u32 initialSize = 128*128*4;
    u8 initialized = 0;

public:
    AtlasArray();
    ~AtlasArray();
    void InsertTexture(u8* texture, glm::vec2* coordinates, u32 width, u32 height);
    void FreeArray();

    u32 GetSize();
    u8* GetAtlasArray();
};


