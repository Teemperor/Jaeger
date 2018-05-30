#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <SFML/Window.hpp>
#include <vector>
#include <Inventory.h>
#include <Vec2.h>

class GameObject;
class Character;

struct InventoryLocation {
  Inventory *Inv = nullptr;
  // Where the inventory is located on the current level.
  Vec2 InvPos = {0, 0};
  InventoryLocation() = default;
  InventoryLocation(Inventory *Inv, Vec2 Pos) : Inv(Inv), InvPos(Pos) {
  }
  bool valid() {
    return Inv != nullptr;
  }
};

class PlayerControls {

  float moveX = 0;
  float moveY = 0;
  int player_;

  GameObject *target = nullptr;
  std::vector<GameObject *> possibleTargets;

  InventoryLocation TargetInv;
  std::vector<InventoryLocation> possibleInventories;

  Character *ControlledChar = nullptr;

public:
  explicit PlayerControls(int player) : player_(player) {}

  void update() {
    moveX = moveY = 0;
    if (player_ == 1) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        moveX += 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        moveX -= 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        moveY -= 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        moveY += 1;
    } else {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveX += 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveX -= 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        moveY -= 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moveY += 1;
    }
  }

  void setControlledCharacter(Character *C) { ControlledChar = C; }

  Character *getControlledCharacter() { return ControlledChar; }

  float getXInput() { return moveX; }
  float getYInput() { return moveY; }

  GameObject *getTarget() { return target; }
  InventoryLocation getInventoryTarget() { return TargetInv; }

  bool isShooting() const;

  void setPossibleTargets(const std::vector<GameObject *> &targets) {
    this->possibleTargets = targets;
    if (target) {
      if (std::find(possibleTargets.begin(), possibleTargets.end(), target) ==
          possibleTargets.end()) {
        target = nullptr;
      }
    }
    if (!target) {
      if (!possibleTargets.empty())
        target = possibleTargets.front();
    }
  }

  void setPossibleInventoryTargets(const std::vector<InventoryLocation> &targets) {
    this->possibleInventories = targets;
    if (!possibleInventories.empty())
      TargetInv = possibleInventories.front();
    else
      TargetInv = InventoryLocation();
  }
};

#endif // PLAYERCONTROLS_H
