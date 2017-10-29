#ifndef ARGOS_GAMEOBJECT_H
#define ARGOS_GAMEOBJECT_H

#include "Vec2.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Level;

class GameObject {

  Vec2 pos;

protected:
  bool ShouldBeRemoved_ = false;
public:
  virtual ~GameObject() {
  }
  virtual void update() {}
  virtual void render(sf::RenderTarget& target) {}
  bool shouldBeRemoved() const {
    return ShouldBeRemoved_;
  }
};

#endif //ARGOS_GAMEOBJECT_H
