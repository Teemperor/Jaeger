#ifndef SPELLEFFECT_H
#define SPELLEFFECT_H

#include <cassert>
#include <vector>
#include <functional>

class Creature;

class SpellEffect  {

public:
  typedef int StrengthUnit;
private:
  std::string ID;
  std::string name;
  std::function<void (Creature*, Creature *, StrengthUnit)> effectUpdate, effectStart, effectEnd;
  StrengthUnit MinStrength, MaxStrength;
  
public:
  SpellEffect(std::string ID, std::string Name,
              std::function<void (Creature*, Creature *, StrengthUnit)> effectUpdate,
              StrengthUnit MinStrength, StrengthUnit MaxStrength);
  SpellEffect(std::string ID, std::string Name,
              std::function<void (Creature*, Creature *, StrengthUnit)> effectStart,
              std::function<void (Creature*, Creature *, StrengthUnit)> effectStop,
              StrengthUnit MinStrength, StrengthUnit MaxStrength);

  void applyStart(Creature* caster, Creature *target, StrengthUnit Strength) const {
    effectStart(caster, target, Strength);
  }

  void applyUpdate(Creature* caster, Creature *target, StrengthUnit Strength) const {
    effectUpdate(caster, target, Strength);
  }

  void applyEnd(Creature* caster, Creature *target, StrengthUnit Strength) const {
    effectEnd(caster, target, Strength);
  }

  const std::string &getID() const {
    return ID;
  }

  const std::string &getName() const {
    return name;
  }
  
  StrengthUnit getMinStrength() const {
    return MinStrength;
  }

  StrengthUnit getMaxStrength() const {
    return MaxStrength;
  }
};

class AppliedSpellEffect {
  SpellEffect *E = nullptr;
  Creature *Caster = nullptr;
  bool HasStarted = false;
  SpellEffect::StrengthUnit Strength = 1;
  unsigned DurationLeft = 1;
  float Time = 0;
public:
  AppliedSpellEffect() {}
  explicit AppliedSpellEffect(SpellEffect &E, Creature *C, unsigned Duration) : E(&E), Caster(C) {
  }

  void update(Creature *Target, float dTime) {
    assert(!finished());
    if (!HasStarted) {
      HasStarted = true;
      E->applyStart(Caster, Target, Strength);
    }
    Time += dTime;
    while (Time >= 1) {
      Time -= 1;
      E->applyUpdate(Caster, Target, Strength);
      DurationLeft--;
      if (DurationLeft == 0) {
        E->applyEnd(Caster, Target, Strength);
      }
    }
  }

  bool finished() const {
    return DurationLeft == 0;
  }
};

namespace SpellEffects {
  extern const std::vector<SpellEffect>& getList();
}

#endif // SPELLEFFECT_H
