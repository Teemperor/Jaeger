#ifndef WORLD_H
#define WORLD_H

#include "Level.h"

#include <vector>

class World {
  std::vector<Level *> levels_;
  GameData& data_;
public:
  World(GameData& d);
  void update(float dtime) {
    for(auto level : levels_) {
      level->update(dtime);
    }
  }

  void addLevel(Level *level) {
    levels_.push_back(level);
  }

  std::vector<Level*>& getLevels() {
    return levels_;
  }
};

#endif // WORLD_H
