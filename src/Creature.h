#ifndef CREATURE_H
#define CREATURE_H

#include <list>

#include "GameObject.h"
#include "SpellEffect.h"
#include "ai/Faction.h"

class Creature : public GameObject {
  Faction *TheFaction = nullptr;
  int Health = 100;
  int MaxHealth = 100;
  int Fatigue = 100;
  int MaxFatigue = 100;
  std::list<AppliedSpellEffect> Effects;

  float FatigueRegTime = 0.1;
  float TimeSinceLastFatigueReg = 0;

public:
  explicit Creature(Level &level, Faction *faction = nullptr);

  void addEffect(const AppliedSpellEffect &E) { Effects.push_back(E); }

  void update(float dtime) override {
    GameObject::update(dtime);
    TimeSinceLastFatigueReg += dtime;
    while (TimeSinceLastFatigueReg > FatigueRegTime) {
      TimeSinceLastFatigueReg -= FatigueRegTime;
      if (Fatigue < MaxFatigue)
        Fatigue++;
    }
  }

  bool trySpendFatigue(int FatiguePointsToSpend) {
    if (Fatigue >= FatiguePointsToSpend) {
      Fatigue -= FatiguePointsToSpend;
      return true;
    }
    return false;
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
    if (!TheFaction)
      return true;
    if (!Other.TheFaction)
      return true;
    return TheFaction->isEnemyFaction(*Other.TheFaction);
  }

  Faction *getFaction() override { return TheFaction; }

  void setFaction(Faction *NewFaction) {
    assert(NewFaction);
    TheFaction = NewFaction;
  }

  void damage(int Dmg) override { Health -= Dmg; }

  int getHealth() const { return Health; }

  int getMaxHealth() const { return MaxHealth; }

  int getFatigue() const { return Fatigue; }

  int getMaxFatigue() const { return MaxFatigue; }

  bool isDead() const { return Health <= 0; }
};

#endif // CREATURE_H
