#ifndef ITEM_H
#define ITEM_H

#include "gamedata/ItemData.h"

class Level;

class Item {
  ItemData *Data_ = nullptr;

  double nextAvailable = 0;

public:
  Item(ItemData &Data);
  Item() {}

  bool empty() const { return Data_ == nullptr; }

  bool available(Level &level);

  bool tryUse(Level &level);

  const sf::Sprite &sprite() { return Data_->sprite(); }

  ItemData::Kind kind() const { return Data_->kind(); }

  double getCooldown() const { return Data_->getCooldown(); }

  bool hasProjectiles() const {
    if (empty())
      return false;
    return static_cast<bool>(getProjectileData());
  }

  float getRange() { return 16 * 10; }

  const ProjectileData *getProjectileData() const {
    return Data_->getProjectileData();
  }

  int armor() const { return Data_->armor(); }

  int attack() const { return Data_->attack(); }

  int value() const { return Data_->value(); }
};

#endif // ITEM_H
