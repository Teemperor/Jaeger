#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <GameObject.h>

class ProjectileData;

class Projectile : public GameObject {

  GameObject *target = nullptr;
  Vec2 targetPos;
  const ProjectileData *data;
  float rotation = 0;

public:
  Projectile(const ProjectileData &data, Level &level, Vec2 startPos,
             GameObject &target);

  void update(float dtime) override;
  void render(sf::RenderTarget &target) override;
};

#endif // PROJECTILE_H
