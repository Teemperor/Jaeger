#ifndef ITEM_H
#define ITEM_H

#include "gamedata/ItemData.h"

class Level;
class World;

class Item {
  ItemData *Data_ = nullptr;
  uint64_t Id_ = 0;

  double nextAvailable = 0;

public:
  explicit Item(ItemData &Data, World& W);
  Item() = default;

  bool empty() const { return Data_ == nullptr; }

  bool available(Level &level);

  bool tryUse(Level &level);

  bool operator==(const Item& I) const {
    if (Id_ == 0)
      return false;
    return I.Id_ == Id_;
  }

  bool operator!=(const Item& I) const {
    return !(I == *this);
  }

  const sf::Sprite &sprite() { return Data_->sprite(); }
  const sf::Sprite &icon() { return Data_->icon(); }

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
