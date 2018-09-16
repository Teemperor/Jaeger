#ifndef MAMBO_ITEMDATA_H
#define MAMBO_ITEMDATA_H

#include "ProjectileData.h"

#include <SFML/Graphics.hpp>

class GameData;

class ItemData {
public:
  enum Kind {
    Pants,
    Helmet,
    Armor,
    Shield,
    Weapon,
    Consumable,
    KindLimit
  };

private:
  int armor_ = 0;
  int attack_ = 0;
  int value_ = 0;
  double cooldown_ = 1;
  sf::Sprite sprite_;
  sf::Sprite icon_;
  std::string projectileName_;
  const ProjectileData *projectileData_ = nullptr;
  Kind kind_;
  std::string id_;

  int FatigueCost = 1;

  const SpellEffect *UseEffect = nullptr;
  mutable RandomRange UseStrength;
  mutable RandomRange UseDuration;

public:
  explicit ItemData(Kind kind) : kind_(kind) {}

  void init(GameData &data);

  Kind kind() { return kind_; }

  void setID(std::string ID) {
    id_ = ID;
  }

  const std::string &getID() const {
    return id_;
  }

  void setUseEffect(const SpellEffect *E, RandomRange S, RandomRange D) {
    UseEffect = E;
    UseStrength = S;
    UseDuration = D;
  }

  bool hasEffect() {
    return UseEffect != nullptr;
  }

  void setFatigueCost(int V) {
    FatigueCost = V;
  }

  int getFatigueCost() const {
    return FatigueCost;
  }

  AppliedSpellEffect getEffect(Creature *User) const {
    assert(UseEffect);
    return AppliedSpellEffect(*UseEffect, User, UseStrength.get(),
                              UseDuration.get());
  }

  const sf::Sprite &sprite() const { return sprite_; }
  void setSprite(const sf::Sprite &sprite) { sprite_ = sprite; }

  const sf::Sprite &icon() const { return icon_; }
  void setIcon(const sf::Sprite &icon) { icon_ = icon; }

  double getCooldown() const { return cooldown_; }

  void setCooldown(double d) { cooldown_ = d; }

  void setProjectileName(const std::string &name) { projectileName_ = name; }

  const ProjectileData *getProjectileData() const { return projectileData_; }

  int armor() const { return armor_; }

  void setArmor(int armor) { armor_ = armor; }

  int attack() const { return attack_; }

  void setAttack(int attack) { attack_ = attack; }

  int value() const { return value_; }

  void setValue(int value) { value_ = value; }
};

#endif // MAMBO_ITEMDATA_H
