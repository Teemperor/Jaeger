#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <SFML/Window.hpp>
#include <vector>

class GameObject;

class PlayerControls {

  float moveX = 0;
  float moveY = 0;
  int player_;
  GameObject *target = nullptr;
  std::vector<GameObject *> possibleTargets;

public:
  PlayerControls(int player) : player_(player) {}

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

  float getXInput() { return moveX; }
  float getYInput() { return moveY; }

  void setPossibleTarget(const std::vector<GameObject *>& targets) {
    this->possibleTargets = targets;
  }
};

#endif // PLAYERCONTROLS_H
