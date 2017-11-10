#ifndef CREATURE_H
#define CREATURE_H

#include "Faction.h"

class Creature {
  Faction *faction_ = nullptr;
  int health_ = 100;
  int maxHealth_ = 100;

public:
  Creature(Faction *faction = nullptr);

  bool isEnemy(Creature &Other) {
    if (&Other == this)
      return false;
    if (!faction_)
      return true;
    if (!Other.faction_)
      return true;
    return faction_->isEnemyFaction(*Other.faction_);
  }

  void setFaction(Faction *faction) { faction_ = faction; }

  void reduceHealth(int dmg) { health_ -= dmg; }

  float percentageHealth() const { return (float)health_ / maxHealth_; }

  bool isDead() const { return health_ <= 0; }
};

#endif // CREATURE_H
