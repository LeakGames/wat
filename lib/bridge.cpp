#include "bridge.h"

lua_State *start_lua(char *fname, Desktop &desk) {
  lua_State *L = luaL_newstate();

  lua_pushlightuserdata(L, &desk);
  lua_setglobal(L, "desktop");

  lua_pushcfunction(L, API_addBox);
  lua_setglobal(L, "addBox");

  lua_pushcfunction(L, API_onClick);
  lua_setglobal(L, "onClick");

  lua_newtable(L);
  SFCOLOR_TO_TABLE(L, sf::Color::Red);
  lua_setglobal(L, "red");

  lua_newtable(L);
  SFCOLOR_TO_TABLE(L, sf::Color::Green);
  lua_setglobal(L, "green");
  
  lua_newtable(L);
  SFCOLOR_TO_TABLE(L, sf::Color::White);
  lua_setglobal(L, "white");

  luaL_openlibs(L);
  luaL_dofile(L, fname);

  return L;
}

void lua_start_file(lua_State *L) {
  if (lua_pcall(L, 0, LUA_MULTRET, 0)) {
      cerr << (string) lua_tostring(L, -1) << endl;
      return;
  }
}

int API_addBox(lua_State *L) {
  int w = luaL_checknumber(L, 1),
      h = luaL_checknumber(L, 2),
      x = luaL_checknumber(L, 3),
      y = luaL_checknumber(L, 4);
  
  luaL_checktype(L, 5, LUA_TTABLE);

  lua_rawgeti(L, 5, 0);
  int r = lua_tonumber(L, -1);

  lua_rawgeti(L, 5, 1);
  int g = lua_tonumber(L, -1);
  
  lua_rawgeti(L, 5, 2);
  int b = lua_tonumber(L, -1);

  lua_getglobal(L, "desktop");
  Desktop *desk = (Desktop *) lua_topointer(L, -1);
  
  //cout << "r: " << r << endl << "g: " << g << endl << "b: " << b << endl;

  sf::RectangleShape *rect = desk->addBox(w, h, x, y, sf::Color(r, g, b));

  lua_pushlightuserdata(L, rect);

  return 1;
}

int API_onClick(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  luaL_checktype(L, 2, LUA_TFUNCTION);

  sf::RectangleShape *rect = (sf::RectangleShape *) lua_topointer(L, 1);
  lua_remove(L, 1);

  lua_getglobal(L, "desktop");
  Desktop *desk = (Desktop *) lua_topointer(L, -1);
  lua_pop(L, 1);

  if (!lua_isfunction(L, -1))
    return luaL_error(L, "There must be only 2 arguments: a shape and a function");

  int cbl = luaL_ref(L, LUA_REGISTRYINDEX);

  desk->onClick(rect, [L, cbl](int x, int y) -> void {
    lua_rawgeti(L, LUA_REGISTRYINDEX, cbl);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    lua_pcall(L, 2, 0, 0);
  });
}
