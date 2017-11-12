#ifndef ARGOS_GAMEOBJECT_H
#define ARGOS_GAMEOBJECT_H

#include "TilePos.h"
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class GameData;
class Level;

class GameObject {

  Vec2 pos;
  Level *level_;

protected:
  bool ShouldBeRemoved_ = false;
  GameObject(Level &l);

public:
  virtual ~GameObject();
  virtual void update(float dtime);
  virtual void render(sf::RenderTarget &target) {}
  bool shouldBeRemoved() const { return ShouldBeRemoved_; }

  Vec2 getPos() const { return pos; }
  TilePos getTilePos() const {
    return TilePos(pos.getX() / 16, pos.getY() / 16);
  }

  virtual void damage(int dmg) {}

  Level &getLevel() { return *level_; }

  void setLevel(Level *l) { level_ = l; }

  GameData &getGameData();

  void setPos(Vec2 p) { pos = p; }
};

#endif // ARGOS_GAMEOBJECT_H
