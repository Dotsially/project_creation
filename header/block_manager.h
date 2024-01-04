#pragma once
#include "depedencies.h"
#include "block_data.h"
#include "sekai_reader.h"
#include <map>
#include <string>

class BlockManager{
private:
    std::map<u8, BlockData> blocks;

    std::map<std::string, u8> blockNames;

    SekaiReader sekaiReader;
public:
    BlockManager();
    ~BlockManager();

    std::map<u8, BlockData>* GetBlocks();
    u8 BlockNameToBlockId(std::string);
};