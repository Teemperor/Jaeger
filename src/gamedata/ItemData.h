#ifndef MAMBO_ITEMDATA_H
#define MAMBO_ITEMDATA_H

#include "ProjectileData.h"

#include <SFML/Graphics.hpp>

class GameData;

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
  double cooldown_ = 1;
  sf::Sprite sprite_;
  std::string projectileName_;
  const ProjectileData* projectileData_ = nullptr;
  Kind kind_;

public:
  ItemData(Kind kind) : kind_(kind) {}

  void init(GameData& data);

  Kind kind() { return kind_; }

  const sf::Sprite &sprite() const { return sprite_; }
  void setSprite(const sf::Sprite &sprite) { sprite_ = sprite; }

  double getCooldown() const { return cooldown_; }

  double setCooldown(double d) { cooldown_ = d; }

  void setProjectileName(const std::string &name) {
    projectileName_ = name;
  }

  const ProjectileData* getProjectileData() const {
    return projectileData_;
  }

  int armor() const { return armor_; }

  void setArmor(int armor) { armor_ = armor; }

  int attack() const { return attack_; }

  void setAttack(int attack) { attack_ = attack; }

  int value() const { return value_; }

  void setValue(int value) { value_ = value; }
};

#endif // MAMBO_ITEMDATA_H
