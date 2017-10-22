#ifndef CHARACTER_H
#define CHARACTER_H

#include <GameObject.h>

class Character : public GameObject {

  sf::Sprite sprite_;

public:
  enum class BodyType {
    Pale,
    Normal,
    Tanned,
    Ork
  };

  Character();

  virtual void render(sf::RenderTarget& target) {

  }

private:
  BodyType BodyType_ = BodyType::Pale;

};

#endif // CHARACTER_H
