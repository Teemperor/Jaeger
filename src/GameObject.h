#ifndef ARGOS_GAMEOBJECT_H
#define ARGOS_GAMEOBJECT_H

#include "Vec2.h"

#include <SFML/Graphics.hpp>

class GameData;
class Level;

class GameObject {

  Vec2 pos;
  Level* level_;

protected:
  bool ShouldBeRemoved_ = false;
  GameObject(Level& l) : level_(&l) {
  }

public:
  virtual ~GameObject();
  virtual void update(float dtime) {}
  virtual void render(sf::RenderTarget& target) {}
  bool shouldBeRemoved() const {
    return ShouldBeRemoved_;
  }

  Vec2 getPos() const {
    return pos;
  }

  Level& getLevel() {
    return *level_;
  }

  GameData& getGameData();

  void setPos(Vec2 p) {
    pos = p;
  }
};

#endif //ARGOS_GAMEOBJECT_H
