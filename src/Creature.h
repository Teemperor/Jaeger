#ifndef CREATURE_H
#define CREATURE_H

#include <list>

#include "Faction.h"
#include "GameObject.h"
#include "SpellEffect.h"

class Creature : public GameObject {
  Faction *TheFaction = nullptr;
  int Health = 100;
  int MaxHealth = 100;
  int Fatigue = 100;
  int MaxFatigue = 100;
  std::list<AppliedSpellEffect> Effects;

public:
  explicit Creature(Level &level, Faction *faction = nullptr);

  void addEffect(const AppliedSpellEffect &E) { Effects.push_back(E); }

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
    if (!TheFaction)
      return true;
    if (!Other.TheFaction)
      return true;
    return TheFaction->isEnemyFaction(*Other.TheFaction);
  }

  void setFaction(Faction *NewFaction) { TheFaction = NewFaction; }

  void damage(int Dmg) override { Health -= Dmg; }

  float percentageHealth() const { return (float)Health / MaxHealth; }

  bool isDead() const { return Health <= 0; }
};

#endif // CREATURE_H
