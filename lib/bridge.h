#ifndef _BRIDGE_H_
#define _BRIDGE_H_

#include <iostream>
#include "desk.h"

#define SFCOLOR_TO_TABLE(L, COLOR) \
  lua_pushinteger(L, 0); \
  lua_pushinteger(L, COLOR.r); \
  lua_settable(L, -3); \
  lua_pushinteger(L, 1); \
  lua_pushinteger(L, COLOR.g); \
  lua_settable(L, -3); \
  lua_pushinteger(L, 2); \
  lua_pushinteger(L, COLOR.b); \
  lua_settable(L, -3);

using namespace std;

extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

lua_State *start_lua(char *, Desktop &);
void lua_start_file(lua_State *);
int API_addBox(lua_State *);
int API_onClick(lua_State *);
#endif
