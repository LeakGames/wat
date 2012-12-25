#include <iostream>
#include <string>

#include "lib/desk.h"
#include "lib/bridge.h" // lua bridge

using namespace std;

int main(int argc, char **argv) {
  Desktop desk(800, 800);
 
  sf::RectangleShape *lal = desk.addBox(100, 100, 100, 100, sf::Color::Green);
  
  // desk->deleteBox(lal);
  desk.threadLoop();

  lua_State *L = start_lua("es.lua", desk);
  lua_start_file(L);

  while (1) {}

  return 0;
}
