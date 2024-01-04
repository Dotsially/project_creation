#pragma once
#include "depedencies.h"

//every mesh type has one.
typedef struct{
    u32 count;
    u32 instanceCount;
    u32 first;
    u32 baseInstance;
} DrawArraysIndirectCommand;

typedef struct{
    u32 count;
    u32 instanceCount;
    u32 firstIndex;
    u32 baseVertex;
    u32 baseInstance;
} DrawElementsIndirectCommand;