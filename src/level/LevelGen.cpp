#include "LevelGen.h"

#include "stb_perlin.h"

Level *LevelGen::generate(GameData &data, int w, int h) {
  this->data = &data;
  level = new Level(w, h, data);


  for (size_t x = 0; x < w; ++x) {
    for (size_t y = 0; y < h; ++y) {
      float f = stb_perlin_noise3(3 * x / (float) w, 3 * y / (float) h, 7);
      std::cout << f << std::endl;
      if (f > -0.2f)
        level->get(x, y).setData(data.tile("grass"));
      else
        level->get(x, y).setData(data.tile("water_c"));
    }
  }
  level->getBuilding(5, 5).setData(data.tile("fireplace"));
  //level->finalize();

  return level;
}
