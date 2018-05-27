#include "ProjectileData.h"

ProjectileData::ProjectileData(const sf::Sprite &sprite,
                               const SpellEffect &hitEffect,
                               RandomRange effectStrength,
                               RandomRange effectDuration)
    : TheSprite(sprite), HitEffect(hitEffect), Strength(effectStrength),
      Duration(effectDuration) {}

AppliedSpellEffect ProjectileData::getEffect(Creature *Caster,
                                             Creature *Target) const {
  return AppliedSpellEffect(HitEffect, Caster, Strength.get(), Duration.get());
}
