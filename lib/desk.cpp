#include "desk.h"

Desktop::Desktop(int h, int w, char *title) {
  this->mode = new sf::VideoMode(h, w);
  this->title = title;
}

sf::RectangleShape *Desktop::addBox(int h, int w, int x, int y, sf::Color color) {
  sf::RectangleShape *box = new sf::RectangleShape(sf::Vector2f(h, w));
  box->setFillColor(color);
  box->setPosition(x, y);

  this->shapes.push_back(box);

  return box;
}

void Desktop::deleteBox(sf::RectangleShape *rec) {
  vector<sf::Shape*>::iterator it = find(this->shapes.begin(), this->shapes.end(), rec);
  this->shapes.erase(it);

  delete *it;
}

void Desktop::onClick(sf::Shape *shape, function<void (int, int)> fn) {
  this->events[shape] = fn;
}

void Desktop::threadLoop() {
  sf::Thread *thread = new sf::Thread(&Desktop::loop, this);

  thread->launch();
}

void Desktop::dispatchMouseEvent(sf::Event &event) {
  int x = event.mouseButton.x,
      y = event.mouseButton.y;

  std::for_each(this->events.begin(), this->events.end(), [x, y] (std::pair< sf::Shape*, function<void(int, int)> > it) {
    if (IN_AREA(x, y, it.first))
      it.second(x, y);
  });
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
    
    for (int n = 0; n < this->shapes.size(); n++)
      this->window->draw(*(this->shapes[n]));

    this->window->display();
  }
}
