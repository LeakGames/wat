#include "bridge.h"

lua_State *start_lua(char *fname, Desktop &desk) {
  lua_State *L = luaL_newstate();

  lua_pushlightuserdata(L, &desk);
  lua_setglobal(L, "desktop");

  lua_pushcfunction(L, API_addBox);
  lua_setglobal(L, "addBox");

  lua_pushcfunction(L, API_addCircle);
  lua_setglobal(L, "addCircle");
  
  lua_pushcfunction(L, API_onClickBox);
  lua_setglobal(L, "onClickBox");

  lua_pushcfunction(L, API_onClickCircle);
  lua_setglobal(L, "onClickCircle");

  lua_pushcfunction(L, API_deleteBox);
  lua_setglobal(L, "deleteBox");

  lua_pushcfunction(L, API_deleteCircle);
  lua_setglobal(L, "deleteCircle");
  
  lua_newtable(L);
  SFCOLOR_TO_TABLE(L, sf::Color::Red);
  lua_setglobal(L, "RED");

  lua_newtable(L);
  SFCOLOR_TO_TABLE(L, sf::Color::Green);
  lua_setglobal(L, "GREEN");
  
  lua_newtable(L);
  SFCOLOR_TO_TABLE(L, sf::Color::White);
  lua_setglobal(L, "WHITE");

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

int API_addCircle(lua_State *L) {
  int x = luaL_checknumber(L, 1),
      y = luaL_checknumber(L, 2),
      radius = luaL_checknumber(L, 3);
  
  luaL_checktype(L, 4, LUA_TTABLE);

  lua_rawgeti(L, 4, 0);
  int r = lua_tonumber(L, -1);

  lua_rawgeti(L, 4, 1);
  int g = lua_tonumber(L, -1);
  
  lua_rawgeti(L, 4, 2);
  int b = lua_tonumber(L, -1);

  lua_getglobal(L, "desktop");
  Desktop *desk = (Desktop *) lua_topointer(L, -1);
  
  //cout << "r: " << r << endl << "g: " << g << endl << "b: " << b << endl;

  sf::CircleShape *circle = desk->addCircle(x, y, radius, sf::Color(r, g, b));

  lua_pushlightuserdata(L, circle);

  return 1;
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

int API_onClickCircle(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  luaL_checktype(L, 2, LUA_TFUNCTION);

  sf::CircleShape *circle = (sf::CircleShape *) lua_topointer(L, 1);
  lua_remove(L, 1);

  lua_getglobal(L, "desktop");
  Desktop *desk = (Desktop *) lua_topointer(L, -1);
  lua_pop(L, 1);

  if (!lua_isfunction(L, -1))
    return luaL_error(L, "There must be only 2 arguments: a shape and a function");

  int cbl = luaL_ref(L, LUA_REGISTRYINDEX);

  desk->onClick(circle, [L, cbl](int x, int y) -> void {
    lua_rawgeti(L, LUA_REGISTRYINDEX, cbl);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    lua_pcall(L, 2, 0, 0);
  });
}

int API_onClickBox(lua_State *L) {
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

int API_deleteBox(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);

  sf::RectangleShape *shape = (sf::RectangleShape *) lua_topointer(L, 1);

  lua_getglobal(L, "desktop");
  Desktop *desk = (Desktop *) lua_topointer(L, -1);
  
  desk->del(shape);

  return 0;
}

int API_deleteCircle(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);

  sf::CircleShape *shape = (sf::CircleShape *) lua_topointer(L, 1);

  lua_getglobal(L, "desktop");
  Desktop *desk = (Desktop *) lua_topointer(L, -1);
  
  desk->del(shape);

  return 0;
}
