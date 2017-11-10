#ifndef PROJECTILEDATA_H
#define PROJECTILEDATA_H

#include <SFML/Graphics.hpp>

class ProjectileData {
  sf::Sprite sprite_;
public:
  ProjectileData(const sf::Sprite &sprite);

  const sf::Sprite& getSprite() const {
    return sprite_;
  }

  float getSpriteRotationSpeed() const {
    return 0;
  }

  float getSpeed() const {
    return 160;
  }
};

#endif // PROJECTILEDATA_H
