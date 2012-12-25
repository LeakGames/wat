#ifndef _DESK_H_
#define _DESK_H_
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>

using namespace std;

#define IN_AREA(x, y, shape) \
  !(x < shape->getPosition().x || \
    y < shape->getPosition().y || \
    x > shape->getPosition().x + reinterpret_cast<sf::RectangleShape *>(shape)->getSize().x || \
    y > shape->getPosition().y + reinterpret_cast<sf::RectangleShape *>(shape)->getSize().y)

class Desktop {
public:
  sf::RenderWindow *window;
  vector<sf::Shape*> shapes;
  sf::VideoMode *mode;
  char *title;
  map<sf::Shape*, function<void(int, int)>> events;

  sf::RectangleShape *addBox(int h, int w, int x, int y, sf::Color color);
  void dispatchMouseEvent(sf::Event &event);
  void onClick(sf::Shape *shape, function<void(int, int)> fn);
  void deleteBox(sf::RectangleShape *rec);
  void loop();
  void threadLoop();
  Desktop(int h = 400, int w = 400, char *title = "nexgay");
};

#endif
