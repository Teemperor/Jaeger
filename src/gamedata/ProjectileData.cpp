#include "ProjectileData.h"

ProjectileData::ProjectileData(const sf::Sprite &sprite, const SpellEffect &hitEffect)
    : sprite_(sprite), hitEffect_(hitEffect) {
}

AppliedSpellEffect ProjectileData::getEffect(Creature *Caster, Creature *Target) const {
  return AppliedSpellEffect(hitEffect_, Caster, 1);
}
