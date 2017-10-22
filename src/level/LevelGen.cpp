#include "LevelGen.h"

Level *LevelGen::generate(GameData &data, int w, int h) {
  this->data = &data;
  level = new Level(w, h, data);

  for (size_t x = 0; x < w; ++x) {
    for (size_t y = 0; y < h; ++y) {
      level->get(x, y).setData(data.tile("grass"));
    }
  }
  level->getBuilding(5, 5).setData(data.tile("fireplace"));
  //level->finalize();

  return level;
}
