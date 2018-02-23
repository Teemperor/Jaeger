#include "SpellEffect.h"

SpellEffect::SpellEffect(std::string ID, std::string Name,
                         std::function<void (Creature*, Creature *, StrengthUnit)> effectUpdate,
                         StrengthUnit MinStrength, StrengthUnit MaxStrength)
    : ID(ID), name(Name), effectUpdate(effectUpdate), MinStrength(MinStrength), MaxStrength(MaxStrength)
{
}

SpellEffect::SpellEffect(std::string ID, std::string Name,
                         std::function<void(Creature *, Creature *, StrengthUnit)> effectStart,
                         std::function<void(Creature *, Creature *, StrengthUnit)> effectEnd,
                         StrengthUnit MinStrength, StrengthUnit MaxStrength)
    : ID(ID), name(Name), effectStart(effectStart), effectEnd(effectEnd), MinStrength(MinStrength),
      MaxStrength(MaxStrength)
{
}

static std::vector<SpellEffect> List;

const std::vector<SpellEffect> &SpellEffects::getList() {
  if (List.empty()) {
    List = {
        SpellEffect("damage", "Damage", [](Creature *Caster, Creature* Target, SpellEffect::StrengthUnit u) {

        }, 1, 100)
    };
  }
  return List;
}
