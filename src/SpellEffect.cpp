#include "SpellEffect.h"
#include "Creature.h"
#include <iostream>

SpellEffect::SpellEffect(
    std::string ID, std::string Name,
    std::function<void(Creature *, Creature *, StrengthUnit)> effectUpdate,
    StrengthUnit MinStrength, StrengthUnit MaxStrength)
    : ID(ID), name(Name), effectUpdate(effectUpdate), MinStrength(MinStrength),
      MaxStrength(MaxStrength) {}

SpellEffect::SpellEffect(
    std::string ID, std::string Name,
    std::function<void(Creature *, Creature *, StrengthUnit)> effectStart,
    std::function<void(Creature *, Creature *, StrengthUnit)> effectEnd,
    StrengthUnit MinStrength, StrengthUnit MaxStrength)
    : ID(ID), name(Name), effectStart(effectStart), effectEnd(effectEnd),
      MinStrength(MinStrength), MaxStrength(MaxStrength) {}

static std::vector<SpellEffect> List;

const std::vector<SpellEffect> &SpellEffects::getList() {
  if (List.empty()) {
    List = {SpellEffect("damage", "Damage",
                        [](Creature *, Creature *Target,
                           SpellEffect::StrengthUnit u) { Target->damage(u); },
                        1, 100),
            SpellEffect("heal", "Heal",
                        [](Creature *, Creature *Target,
                           SpellEffect::StrengthUnit u) { Target->damage(-u); },
                        1, 100),
            SpellEffect("switchPos", "Switch Positions",
                        [](Creature *Caster, Creature *Target,
                           SpellEffect::StrengthUnit) {
                          auto pos = Target->getPos();
                          Target->setPos(Caster->getPos());
                          Caster->setPos(pos);
                        },
                        1, 100)};
  }
  return List;
}
const SpellEffect & ::SpellEffects::getByID(const std::string &ID) {
  for (auto &E : getList()) {
    if (E.getID() == ID)
      return E;
  }
  std::cerr << "Couldn't find effect with ID '" << ID << "'\n";
  assert(false);
  return getList().front();
}
