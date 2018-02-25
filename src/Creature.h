#ifndef CREATURE_H
#define CREATURE_H

#include <list>

#include "Faction.h"
#include "SpellEffect.h"
#include "GameObject.h"

class Creature : public GameObject {
  Faction *faction_ = nullptr;
  int health_ = 100;
  int maxHealth_ = 100;
  int fatigue_ = 100;
  int maxFatigue_ = 100;
  std::list<AppliedSpellEffect> Effects;

public:
  Creature(Level &level, Faction *faction = nullptr);

  void addEffect(const AppliedSpellEffect& E) {
    Effects.push_back(E);
  }

  void updateEffects(float DeltaTime) {
    for (auto Iter = Effects.begin(); Iter != Effects.end();) {
      AppliedSpellEffect &E = *Iter;
      E.update(this, DeltaTime);
      if (E.finished()) {
        Iter = Effects.erase(Iter);
      } else {
        ++Iter;
      }
    }
  }

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

  virtual void damage(int dmg) { health_ -= dmg; }

  float percentageHealth() const { return (float)health_ / maxHealth_; }

  bool isDead() const { return health_ <= 0; }
};

#endif // CREATURE_H
