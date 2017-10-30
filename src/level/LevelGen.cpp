#include "LevelGen.h"

#include <random>

#include "stb_perlin.h"

void LevelGen::make_tree(int x, int y) {
  if (!level->getBuilding(x, y).empty())
    return;
  if (!level->getBuilding(x, y - 1).empty())
    return;
  level->getBuilding(x, y).setData(data->tile("tree_green_lower"));
  level->getBuilding(x, y - 1).setData(data->tile("tree_green_upper"));
}

void LevelGen::make_bush(int x, int y, float random) {
  std::string tileName = "small_bush";
  if(random > 0.8f)
    tileName = "flower";
  if (!level->getBuilding(x, y).empty())
    return;
  level->getBuilding(x, y).setData(data->tile(tileName));
}


Level *LevelGen::generate(GameData &data, int w, int h) {
  this->data = &data;
  level = new Level(w, h, data);


  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0, 1);

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float vegetation = stb_perlin_noise3(3 * x / (float) w, 42, 3 * y / (float) h);
      float height = stb_perlin_noise3(3 * x / (float) w, 3 * y / (float) h, 7);
      height += 0.2f;
      if (height > 0) {
        level->get(x, y).setData(data.tile("grass"));
        if(vegetation > 0.3f && dis(gen) > 0.7f) {
          make_tree(x, y);
        } else if(vegetation > 0 && dis(gen) > 0.8f) {
          //make_bush(x, y);
        } else if(dis(gen) > 0.8f) {
          make_bush(x, y, dis(gen));
        }
      } else {
        level->get(x, y).setData(data.tile("water_c"));
        if(height > -0.1f && vegetation > 0 && dis(gen) > 0.8f) {
          level->getBuilding(x, y).setData(data.tile("lilypad"));
        }
      }
    }
  }
  level->getBuilding(5, 5).setData(data.tile("fireplace"));
  //level->finalize();

  return level;
}
