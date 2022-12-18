#pragma once
#include <string>
#include <vector>

void lua_pushtableEx(lua_State* L, void* lpBuffer, DWORD count);
void lua_pushtableEx(lua_State* L, COleSafeArray& ary);
void lua_pushString(lua_State* L, std::string ary);
void lua_pushVectorString(lua_State* L, std::vector<std::string> ary);
COleSafeArray lua_totableEx(lua_State* L, int no);




