#include "World.h"

World::World(GameData &d) : data_(d) {}

void World::update(float dtime) {
  for (auto level : levels_) {
    level->update(dtime);
  }
  for (auto &tt : ToTeleport) {
    tt.first->getLevel().remove(tt.first);
    tt.second.getLevel().add(tt.first);
    tt.first->setLevel(&tt.second.getLevel());
    tt.first->setPos(Vec2(tt.second));
  }
  ToTeleport.clear();
}
