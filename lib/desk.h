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

#define DISTANCE(x1, y1, x2, y2) \
  sqrt(pow(y1 - y2, 2) + pow(x1 - x2, 2))

#define CIRCLE_IN_AREA(x1, y1, x2, y2, r) \
  DISTANCE(x1, y1, x2, y2) <= r

#define RECT_IN_AREA(x, y, shape) \
  !(x < shape->getPosition().x || \
    y < shape->getPosition().y || \
    x > shape->getPosition().x + reinterpret_cast<sf::RectangleShape *>(shape)->getSize().x || \
    y > shape->getPosition().y + reinterpret_cast<sf::RectangleShape *>(shape)->getSize().y)

class Desktop {
public:
  sf::RenderWindow *window;
  sf::VideoMode *mode;
  char *title;

  vector<sf::CircleShape*> circles;
  vector<sf::RectangleShape*> rectangles;
  
  map<sf::CircleShape*, function<void(int, int)>> circleEvents;
  map<sf::RectangleShape*, function<void(int, int)>> rectangleEvents;

  sf::RectangleShape *addBox(int x, int y, int w, int h, sf::Color color);
  sf::CircleShape *addCircle(int x, int y, int r, sf::Color color);
  
  void dispatchMouseEvent(sf::Event &event);
  
  void onClick(sf::RectangleShape *shape, function<void(int, int)> fn);
  void onClick(sf::CircleShape *shape, function<void(int, int)> fn);
  
  void del(sf::RectangleShape *rec);
  void del(sf::CircleShape *rec);

  void loop();
  void threadLoop();
  
  Desktop(int h = 400, int w = 400, char *title = "nexgay");
};

#endif
