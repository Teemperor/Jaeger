#include <Creature.h>
#include "Projectile.h"

#include "gamedata/ProjectileData.h"

Projectile::Projectile(const ProjectileData &data, Level &level, Vec2 startPos,
                       Creature &source, GameObject &target)
    : GameObject(level), target(&target), source(&source), data(&data) {
  setPos(startPos);
  targetPos = target.getPos();

  ToTarget = targetPos - getPos();
  rotation = std::atan2(ToTarget.getY(), ToTarget.getX());
  ToTarget = Vec2(std::cos(rotation), std::sin(rotation));
}

void Projectile::update(float dtime) {
  setPos(getPos() + (ToTarget * dtime * data->getSpeed()));

  float HitDistance = 16;
  float newDistance = targetPos.distance(getPos());

  if (target->getPos().distance(getPos()) < HitDistance) {
    ShouldBeRemoved_ = true;
    if (auto C = dynamic_cast<Creature *>(target)) {
      C->addEffect(data->getEffect(source, C));
    }
  }

  if (newDistance - 0.1f > distanceToTarget) {
    ShouldBeRemoved_ = true;
  }

  distanceToTarget = newDistance;
}

void Projectile::render(sf::RenderTarget &target) {
  sf::Sprite s = data->getSprite();
  s.setPosition(getPos().getX(), getPos().getY());
  s.setRotation(rotation * 57.2958f);
  target.draw(s);
}
