#ifndef MAMBO_ITEMDATA_H
#define MAMBO_ITEMDATA_H

#include <SFML/Graphics.hpp>

class ItemData {
public:

  enum Kind {
    Pants = 0,
    Helmet = 1,
    Armor = 2,
    Shield = 3,
    Weapon = 4,
    KindLimit = 5
  };
private:
  int armor_ = 0;
  int attack_ = 0;
  int value_ = 0;
  sf::Sprite sprite_;
  Kind kind_;

public:
  ItemData(Kind kind) : kind_(kind) {
  }

  Kind kind() {
    return kind_;
  }

  sf::Sprite& sprite() {
    return sprite_;
  }
  void setSprite(const sf::Sprite& sprite) {
    sprite_ = sprite;
  }

  int armor() const {
    return armor_;
  }

  void setArmor(int armor) {
    armor_ = armor;
  }

  int attack() const {
    return attack_;
  }

  void setAttack(int attack) {
    attack_ = attack;
  }

  int value() const {
    return value_;
  }

  void setValue(int value) {
    value_ = value;
  }

};


#endif //MAMBO_ITEMDATA_H
