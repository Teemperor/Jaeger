#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <vector>
#include <SFML/Window.hpp>

class PlayerControls {

  float moveX = 0;
  float moveY = 0;

public:
    void update() {
      moveX = moveY = 0;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
          moveX += 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
          moveX -= 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
          moveY -= 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
          moveY += 1;
    }

    float getXInput() {
      return moveX;
    }
    float getYInput() {
      return moveY;
    }
};

#endif // PLAYERCONTROLS_H
