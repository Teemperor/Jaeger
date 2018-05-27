#ifndef ARGOS_GAMEOBJECT_H
#define ARGOS_GAMEOBJECT_H

#include "TilePos.h"
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class GameData;
class Level;
class Inventory;

class GameObject {

  Vec2 Pos;
  Level *TheLevel;
  Inventory *Inv = nullptr;

protected:
  bool ShouldBeRemoved = false;
  explicit GameObject(Level &l);
  void setInventory(Inventory *I) {
    Inv = I;
  }

public:
  virtual ~GameObject();
  virtual void update(float dtime);
  virtual void render(sf::RenderTarget &target) {}
  bool shouldBeRemoved() const { return ShouldBeRemoved; }

  Vec2 getPos() const { return Pos; }
  TilePos getTilePos() const {
    return TilePos(Pos.getX() / 16, Pos.getY() / 16);
  }

  Inventory *getInventory() {
    return Inv;
  }

  virtual void damage(int Dmg) {}

  Level &getLevel() { return *TheLevel; }

  void setLevel(Level *L) { TheLevel = L; }

  GameData &getGameData();

  void setPos(Vec2 p) { Pos = p; }
};

#endif // ARGOS_GAMEOBJECT_H
