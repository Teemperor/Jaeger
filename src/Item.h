#ifndef ITEM_H
#define ITEM_H

#include "gamedata/ItemData.h"

class Level;
class World;

class Item {
  ItemData *Data = nullptr;
  uint64_t ID = 0;

  double NextAvailableTime = 0;

public:
  explicit Item(ItemData &Data, World &W);
  Item() = default;

  bool empty() const { return Data == nullptr; }

  bool available(Level &level);

  bool tryUse(Level &level);

  bool operator==(const Item &I) const {
    if (ID == 0)
      return false;
    return I.ID == ID;
  }

  bool operator!=(const Item &I) const { return !(I == *this); }

  AppliedSpellEffect getEffect(Creature *User) const {
    return Data->getEffect(User);
  }

  int getFatigueCost() const {
    return Data->getFatigueCost();
  }

  const sf::Sprite &sprite() { return Data->sprite(); }
  const sf::Sprite &icon() { return Data->icon(); }

  ItemData::Kind kind() const { return Data->kind(); }

  double getCooldown() const { return Data->getCooldown(); }

  bool hasProjectiles() const {
    if (empty())
      return false;
    return static_cast<bool>(getProjectileData());
  }

  float getRange() { return 16 * 10; }

  const ProjectileData *getProjectileData() const {
    return Data->getProjectileData();
  }

  int armor() const { return Data->armor(); }

  int attack() const { return Data->attack(); }

  int value() const { return Data->value(); }
};

#endif // ITEM_H
