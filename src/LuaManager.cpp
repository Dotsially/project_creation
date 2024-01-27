#include "lua_manager.h"
#include "iostream"



u8 LuaManager::CheckLua(lua_State* L, i32 result){
    if(result != LUA_OK){
        std::string errorMsg = lua_tostring(L, -1);
        std::cout << errorMsg << std::endl;
        return 0;
        // lua_getglobal(L, "a");
        // if(lua_isnumber(L, -1)){
        //     std::cout << (int)lua_tonumber(L, -1) << std::endl;            
        // }
    }
    return 1;
}



std::string LuaManager::GetStringVariable(lua_State* L, std::string variableName){
    std::string variable = "";
    lua_pushstring(L, variableName.c_str());
    lua_gettable(L, -2);
    variable = lua_tostring(L, -1);
    lua_pop(L, 1);

    return variable;
}

template<typename T>
void LuaManager::IterateNumberTable(lua_State* L, std::string tableName, T* array){
    lua_pushstring(L, tableName.c_str());
    lua_gettable(L, -2);
    // stack now contains: -1 => table
    lua_pushnil(L);
    // stack now contains: -1 => nil; -2 => table
    i32 i = 0;
    while (lua_next(L, -2))
    {
        // stack now contains: -1 => value; -2 => key; -3 => table
        // copy the key so that lua_tostring does not modify the original
        lua_pushvalue(L, -2);
        // stack now contains: -1 => key; -2 => value; -3 => key; -4 => table
        i32 value = (T)lua_tonumber(L, -2);
        array[i] = value;
        i++;
        // pop value + copy of key, leaving original key
        lua_pop(L, 2);
        // stack now contains: -1 => key; -2 => table
    }
    // stack now contains: -1 => table (when lua_next returns 0 it pops the key
    // but does not push anything.)
    // Pop table
    lua_pop(L, 1);
    // Stack is now the same as it was on entry to this function
}

void LuaManager::IterateStringTable(lua_State* L, std::string tableName, std::string* array){
    lua_pushstring(L, tableName.c_str());
    lua_gettable(L, -2);
    // stack now contains: -1 => table
    lua_pushnil(L);
    // stack now contains: -1 => nil; -2 => table
    i32 i = 0;
    while (lua_next(L, -2))
    {
        // stack now contains: -1 => value; -2 => key; -3 => table
        // copy the key so that lua_tostring does not modify the original
        lua_pushvalue(L, -2);
        // stack now contains: -1 => key; -2 => value; -3 => key; -4 => table
        std::string value = lua_tostring(L, -2);
        array[i] = value;
        i++;
        // pop value + copy of key, leaving original key
        lua_pop(L, 2);
        // stack now contains: -1 => key; -2 => table
    }
    // stack now contains: -1 => table (when lua_next returns 0 it pops the key
    // but does not push anything.)
    // Pop table
    lua_pop(L, 1);
    // Stack is now the same as it was on entry to this function
}


template<typename T>
T LuaManager::GetNumberVariable(lua_State* L, std::string variableName){
    T variable = 0;
    lua_pushstring(L, variableName.c_str());
    lua_gettable(L, -2);
    variable = (T)lua_tonumber(L, -1);
    lua_pop(L, 1);

    return variable;
}
