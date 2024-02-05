#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"
#include "time.h"
#include <vector>

typedef struct BSPLeaf{
    u32 width;
    u32 height;
    glm::ivec2 startPosition;
    glm::ivec2 center;
} BSPLeaf;


template <typename T>
class Leaf{
public:
    T data;
    Leaf* parent = nullptr;
    Leaf* child1 = nullptr;
    Leaf* child2 = nullptr;
};


class BSPTree{
private:    
    std::vector<Leaf<BSPLeaf>> leafs;
    void PushLeaf(Leaf<BSPLeaf> child);
public:    
    void CreateBSP(u32 width, u32 height, u32 partitions);
    void PrintTree();
    std::vector<Leaf<BSPLeaf>> GetLeaves();
    i32 treeIndex = 0;
};

