#include "LevelGen.h"

#include <random>

#include "stb_perlin.h"

void LevelGen::make_tree(int x, int y) {
  if (!level->getBuilding(x, y).empty())
    return;
  if (!level->getBuilding(x, y - 1).empty())
    return;
  level->getBuilding(x, y).setData(data->tile("tree_green_lower"));
  level->getOverlay(x, y - 1).setData(data->tile("tree_green_upper"));
}

void LevelGen::make_bush(int x, int y, float random) {
  std::string tileName = "small_bush";
  if(random > 0.8f)
    tileName = "flower";
  if (!level->getBuilding(x, y).empty())
    return;
  level->getBuilding(x, y).setData(data->tile(tileName));
}

void LevelGen::make_house(int x, int y, int w, int h, int depth) {

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0, 1);

  for(int ix = x; ix < x + w; ix++) {
    for(int iy = y + 1; iy < y + h; iy++) {
      if(ix == x) {
        build(ix, iy, "sand_wall_left");
      } else if(ix == x + w - 1) {
        build(ix, iy, "sand_wall_right");
      } else if(iy == y + h - 1) {
        build(ix, iy, "sand_wall_lower_mid");
      } else {
        build(ix, iy, "sand_wall_mid");
      }
    }
  }

  for(int ix = x; ix < x + w; ix++) {
    for(int iy = y; iy < y + h; iy++) {
      if (iy == y) {
        overlay(ix, iy, "brown_roof_up");
      } else if(ix == x && iy < y + h - depth) {
        overlay(ix, iy, "brown_roof_angular_left_mid");
      } else if(ix == x + w - 1 && iy < y + h - depth) {
        overlay(ix, iy, "brown_roof_angular_right_mid");
      } else if(iy < y + h - depth - 1) {
        overlay(ix, iy, "brown_roof_mid");
      } else if(iy > y + h - depth - 1
                && iy < y + h - 1
                && ix > x
                && ix < x + w - 1
                && ix % 2 == 0
                && iy % 2 == 0) {
        if(dis(gen) > 0.0f) {
          overlay(ix, iy, "brown_window");
        }
      }
    }
  }
  overlay(x, y + h - 1 - depth, "brown_roof_angular_left_lower");
  overlay(x + w - 1, y + h  - 1 - depth, "brown_roof_angular_right_lower");
  overlay(x, y, "brown_roof_angular_left_upper");
  overlay(x + w - 1, y, "brown_roof_angular_right_upper");
  build(x, y + h - 1, "sand_wall_lower_left");
  build(x + w - 1, y + h - 1, "sand_wall_lower_right");
}

void LevelGen::build(int x, int y, std::string tileName) {
  level->getBuilding(x, y).setData(data->tile(tileName));
}

void LevelGen::overlay(int x, int y, std::string tileName) {
  level->getOverlay(x, y).setData(data->tile(tileName));
}

Level *LevelGen::generate(GameData &data, int w, int h) {
  this->data = &data;
  level = new Level(w, h, data);


  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0, 1);

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float height = stb_perlin_noise3(3 * x / (float) w, 3 * y / (float) h, 7);
      height += 0.2f;
      if (height > 0) {
        level->get(x, y).setData(data.tile("grass"));
      } else {
        level->get(x, y).setData(data.tile("water_c"));
      }
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float vegetation = stb_perlin_noise3(3 * x / (float) w, 42, 3 * y / (float) h);
      float height = stb_perlin_noise3(3 * x / (float) w, 3 * y / (float) h, 7);
      height += 0.2f;
      if (height > 0) {
        if(vegetation > 0.3f && dis(gen) > 0.7f) {
          make_tree(x, y);
        } else if(vegetation > 0 && dis(gen) > 0.8f) {
          //make_bush(x, y);
        } else if(dis(gen) > 0.8f) {
          make_bush(x, y, dis(gen));
        }
      } else {
        if(height > -0.1f && vegetation > 0 && dis(gen) > 0.8f) {
          level->getBuilding(x, y).setData(data.tile("lilypad"));
        }
      }
    }
  }
  level->getBuilding(5, 5).setData(data.tile("fireplace"));
  //level->finalize();

  make_house(8, 8, 6, 8, 3);

  return level;
}
