#include "desk.h"

Desktop::Desktop(int h, int w, char *title) {
  this->mode = new sf::VideoMode(h, w);
  this->title = title;
}

sf::RectangleShape *Desktop::addBox(int x, int y, int w, int h, sf::Color color) {
  sf::RectangleShape *box = new sf::RectangleShape(sf::Vector2f(h, w));
  box->setFillColor(color);
  box->setPosition(x, y);

  this->rectangles.push_back(box);

  return box;
}

void Desktop::del(sf::RectangleShape *rec) {
  vector<sf::RectangleShape*>::iterator it = find(this->rectangles.begin(), this->rectangles.end(), rec);

  if (it == this->rectangles.end())
    return;

  this->rectangleEvents.erase(rec);
  this->rectangles.erase(it);

  delete *it;
}

void Desktop::del(sf::CircleShape *rec) {
  vector<sf::CircleShape*>::iterator it = find(this->circles.begin(), this->circles.end(), rec);

  if (it == this->circles.end())
    return;

  this->circleEvents.erase(rec);
  this->circles.erase(it);

  delete *it;
}

void Desktop::onClick(sf::RectangleShape *shape, function<void(int, int)> fn) {
  if (std::find(this->rectangles.begin(), this->rectangles.end(), shape) == this->rectangles.end()) // use-after-free free
    return;

  this->rectangleEvents[shape] = fn;
}

void Desktop::onClick(sf::CircleShape *shape, function<void (int, int)> fn) {
  if (std::find(this->circles.begin(), this->circles.end(), shape) == this->circles.end()) // use-after-free free
    return;
  
  this->circleEvents[shape] = fn;
}

void Desktop::threadLoop() {
  sf::Thread *thread = new sf::Thread(&Desktop::loop, this);

  thread->launch();
}

void Desktop::dispatchMouseEvent(sf::Event &event) {
  int x = event.mouseButton.x,
      y = event.mouseButton.y;

  std::for_each(this->rectangleEvents.begin(), this->rectangleEvents.end(), [x, y] (std::pair< sf::RectangleShape*, function<void(int, int)> > it) {
    if (RECT_IN_AREA(x, y, it.first))
      it.second(x, y);
  });


  std::for_each(this->circleEvents.begin(), this->circleEvents.end(), [x, y] (std::pair< sf::CircleShape*, function<void(int, int)> > it) {
    sf::CircleShape *circle = it.first;

    int centerx = circle->getPosition().x + circle->getRadius(),
        centery = circle->getPosition().y + circle->getRadius();

    if (CIRCLE_IN_AREA(x, y, centerx, centery, circle->getRadius()))
      it.second(x, y);
  });
}

sf::CircleShape *Desktop::addCircle(int x, int y, int r, sf::Color color) {
  sf::CircleShape *circle = new sf::CircleShape();

  circle->setRadius(r);
  circle->setPosition(x, y);
  circle->setFillColor(color);

  this->circles.push_back(circle);

  return circle;
}

void Desktop::loop() {
  this->window = new sf::RenderWindow(*(this->mode), this->title);
  
  while (this->window->isOpen()) {
    sf::Event event;
  
    while (this->window->pollEvent(event)) {
      switch(event.type) {
        case sf::Event::Closed:
          this->window->close();
          return;

        case sf::Event::MouseButtonPressed:
          this->dispatchMouseEvent(event);
          break;
      }
    }

    this->window->clear();
    
    std::for_each(this->rectangles.begin(), this->rectangles.end(), [&] (sf::RectangleShape *shape) {
      this->window->draw(*shape);
    });

    std::for_each(this->circles.begin(), this->circles.end(), [&] (sf::CircleShape *shape) {
      this->window->draw(*shape);
    });

    this->window->display();
  }
}
