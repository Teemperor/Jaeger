#include "ProjectileData.h"

ProjectileData::ProjectileData(const sf::Sprite &sprite, const SpellEffect &hitEffect,
                               unsigned effectStrength, unsigned effectDuration)
    : sprite_(sprite), hitEffect_(hitEffect), strength(effectStrength), duration(effectDuration) {
}

AppliedSpellEffect ProjectileData::getEffect(Creature *Caster, Creature *Target) const {
  return AppliedSpellEffect(hitEffect_, Caster, 1);
}
