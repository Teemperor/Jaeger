#ifndef WORLD_H
#define WORLD_H

#include "Level.h"

#include <vector>

class GameObject;

class World {
  std::vector<Level *> levels_;
  GameData& data_;
  std::vector<std::pair<GameObject*, TilePos> > ToTeleport;
public:
  World(GameData& d);
  void update(float dtime);

  void queryTeleport(GameObject* o, TilePos target) {
    ToTeleport.push_back(std::make_pair(o, target));
  }

  void addLevel(Level *level) {
    levels_.push_back(level);
  }

  std::vector<Level*>& getLevels() {
    return levels_;
  }
};

#endif // WORLD_H
