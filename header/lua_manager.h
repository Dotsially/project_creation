#pragma once
#include "depedencies.h"
#include <string>

extern "C"{
    #include "lua/lua.h"
    #include "lua/lauxlib.h"
    #include "lua/lualib.h"
}


class LuaManager{

public: 
    u8 CheckLua(lua_State* L, i32 result);
    
    
    template<typename T>
    void IterateNumberTable(lua_State* L, std::string tableName, T* array);   
    void IterateStringTable(lua_State* L, std::string tableName, std::string* array);
    std::string GetStringVariable(lua_State* L, std::string variableName);
    template<typename T>
    T GetNumberVariable(lua_State* L, std::string variableName);
};