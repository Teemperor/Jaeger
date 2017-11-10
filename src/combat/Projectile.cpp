#include "Projectile.h"

#include "gamedata/ProjectileData.h"

Projectile::Projectile(const ProjectileData &data, Level &level, Vec2 startPos, GameObject &target)
  : GameObject(level), target(&target), data(&data)
{
  assert(&data && "No data supplied?");
  assert(&target && "No target supplied?");
  setPos(startPos);
  targetPos = target.getPos();

  Vec2 toTarget = targetPos - getPos();
  rotation = std::atan2(toTarget.getY(), toTarget.getX());
}

void Projectile::update(float dtime) {
  Vec2 toTarget = targetPos - getPos();
  float direction = std::atan2(toTarget.getY(), toTarget.getX());
  toTarget = Vec2(std::cos(direction), std::sin(direction));
  setPos(getPos() + (toTarget * dtime * data->getSpeed()));
  if (targetPos.distance(getPos()) < 5) {
    target->damage(8);
    ShouldBeRemoved_ = true;
  }
}

void Projectile::render(sf::RenderTarget &target) {
  sf::Sprite s = data->getSprite();
  s.setPosition(getPos().getX(), getPos().getY());
  s.setRotation(rotation * 57.2958f);
  target.draw(s);
}
