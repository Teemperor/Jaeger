#ifndef PROJECTILEDATA_H
#define PROJECTILEDATA_H

#include <SFML/Graphics.hpp>

#include <SpellEffect.h>

class ProjectileData {
  sf::Sprite sprite_;
  const SpellEffect &hitEffect_;
  mutable RandomRange strength;
  mutable RandomRange duration;

public:
  ProjectileData(const sf::Sprite &sprite, const SpellEffect &hitEffect,
                 RandomRange effectStrength, RandomRange effectDuration);

  const sf::Sprite &getSprite() const { return sprite_; }

  float getSpriteRotationSpeed() const { return 0; }

  float getSpeed() const { return 160; }

  AppliedSpellEffect getEffect(Creature *Caster, Creature *Target) const;
};

#endif // PROJECTILEDATA_H
