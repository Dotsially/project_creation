#include "binary_tree.h"
#include <iostream>


void BSPTree::CreateBSP(u32 width, u32 height, u32 partitions){
    Leaf<BSPLeaf> leaf;
    leaf.data.startPosition = glm::vec2(0,0);
    leaf.data.width = width;
    leaf.data.height = height;
    leaf.data.center = glm::vec2(width/2.0, height/2.0);
    
    leafs.push_back(leaf);

    Leaf<BSPLeaf> child1;
    Leaf<BSPLeaf> child2;

    
    srand(time(0));
    u8 direction = 0;
    for(i32 i = 0; i < partitions; i++){
        child1.parent = &leafs[treeIndex];
        child2.parent = &leafs[treeIndex];

        u8 plusMinus = rand() % 2;
        
        if(i == 1){
            direction = !direction;
        }
        else if(i > 2){
            direction = !direction;
        }   


        u32 valueH = rand() % leafs[treeIndex].data.width/10.0;
        u32 valueV = rand() % leafs[treeIndex].data.height/10.0;
        
        glm::ivec2 slice;
        if(plusMinus && direction){
                slice = glm::vec2(child1.parent->data.startPosition.x, 
                    leafs[treeIndex].data.center.y + valueV);
        }else if(plusMinus){
                slice = glm::vec2(leafs[treeIndex].data.center.x + valueH, 
                    child1.parent->data.startPosition.y);
        }else if(direction){
                slice = glm::vec2(child1.parent->data.startPosition.x, 
                    leafs[treeIndex].data.center.y - valueV);
        }else{
                slice = glm::vec2(leafs[treeIndex].data.center.x - valueH, 
                    child1.parent->data.startPosition.y);
        }

        child1.data.startPosition = child1.parent->data.startPosition;
        child2.data.startPosition = slice;

        if(direction){
            child1.data.width = child1.parent->data.width;
            child1.data.height = slice.y - child1.data.startPosition.y;
            child2.data.width = child2.parent->data.width;
            child2.data.height = child2.parent->data.startPosition.y + child2.parent->data.height - child2.data.startPosition.y;
        }else{
            child1.data.width =  slice.x - child1.data.startPosition.x;
            child1.data.height = child1.parent->data.height;
            child2.data.width = child2.parent->data.startPosition.x + child2.parent->data.width - child2.data.startPosition.x;
            child2.data.height = child2.parent->data.height;
        }


        child1.data.center = glm::vec2(child1.data.startPosition.x + child1.data.width/2.0, 
            child1.data.startPosition.y + child1.data.height/2.0);


        child2.data.center = glm::vec2(child2.data.startPosition.x + child2.data.width/2.0, 
            child2.data.startPosition.y + child2.data.height/2.0);    
        
        PushLeaf(child1);
        PushLeaf(child2);
        
    }

}


void BSPTree::PushLeaf(Leaf<BSPLeaf> child){
    u8 inserted = 0;
    while(!inserted){
        if(leafs[treeIndex].child1 == nullptr){
            leafs.push_back(child);
            leafs[treeIndex].child1 = &leafs.back();
            inserted = 1;
            continue;
        }
        else if(leafs[treeIndex].child2 == nullptr){
            leafs.push_back(child);
            leafs[treeIndex].child2 = &leafs.back();
            inserted = 1;
            treeIndex++;
            continue;
        }
    }
}

void BSPTree::PrintTree(){
    i32 i = 0;
    for(auto leaf: leafs){
        std::cout << "index: " << i << std::endl;
        std::cout << "start position: " << leaf.data.startPosition.x << " " << leaf.data.startPosition.y << std::endl;
        std::cout << "width: " << leaf.data.width << " height: " << leaf.data.height << std::endl;
        std::cout << "center: " << leaf.data.center.x << " " << leaf.data.center.y << std::endl;
        std::cout << "child1: " << leaf.child1 << " child2: " << leaf.child2 << std::endl;
        i++;

    }
}

std::vector<Leaf<BSPLeaf>> BSPTree::GetLeaves(){
    return leafs;
}