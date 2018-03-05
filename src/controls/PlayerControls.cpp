#include "PlayerControls.h"

bool PlayerControls::isShooting() const {
  if (player_ == 1)
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
  if (player_ == 2)
    return sf::Keyboard::isKeyPressed(sf::Keyboard::J);
  return false;
}
