#include "LevelGen.h"

#include "World.h"

#include "stb_perlin.h"

void LevelGen::make_tree(int x, int y, bool force) {
  if (!force) {
    if (!level->getBuilding(x, y).empty())
      return;
  }
  std::string isDark = "";
  if (dis(gen) > 0.5f)
    isDark = "dark";

  if (dis(gen) > 0.1f) {
    if (dis(gen) > 0.5f) {
      level->getBuilding(x, y).setData(data->tile("tree_" + isDark + "green_lower"));
      level->getOverlay(x, y - 1).setData(data->tile("tree_" + isDark + "green_upper"));
    } else {
      level->getBuilding(x, y).setData(data->tile("pine_" + isDark + "green_lower"));
      level->getOverlay(x, y - 1).setData(data->tile("pine_" + isDark + "green_upper"));
    }
  } else {
    level->getBuilding(x, y).setData(data->tile("tree_" + isDark + "green_small"));
  }
}

void LevelGen::make_bush(int x, int y, float random) {
  std::string tileName = "small_bush";
  if (random > 0.8f)
    tileName = "flower";
  if (!level->getBuilding(x, y).empty())
    return;
  level->getBuilding(x, y).setData(data->tile(tileName));
}

void LevelGen::make_house(int x, int y, int w, int h, int depth) {

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0, 1);

  for (int ix = x; ix < x + w; ix++) {
    for (int iy = y + 1; iy < y + h; iy++) {
      if (ix == x) {
        build(ix, iy, "sand_wall_left");
      } else if (ix == x + w - 1) {
        build(ix, iy, "sand_wall_right");
      } else if (iy == y + h - 1) {
        build(ix, iy, "sand_wall_lower_mid");
      } else {
        build(ix, iy, "sand_wall_mid");
      }
    }
  }

  for (int ix = x; ix < x + w; ix++) {
    for (int iy = y; iy < y + h; iy++) {
      if (iy == y) {
        overlay(ix, iy, "brown_roof_up");
      } else if (ix == x && iy < y + h - depth) {
        overlay(ix, iy, "brown_roof_angular_left_mid");
      } else if (ix == x + w - 1 && iy < y + h - depth) {
        overlay(ix, iy, "brown_roof_angular_right_mid");
      } else if (iy < y + h - depth - 1) {
        overlay(ix, iy, "brown_roof_mid");
      } else if (iy > y + h - depth - 1 && iy < y + h - 1 && ix > x &&
                 ix < x + w - 1 && ix % 2 == 0 && iy % 2 == 0) {
        if (dis(gen) > 0.0f) {
          overlay(ix, iy, "brown_window");
        }
      }
    }
  }
  int doorX = x + (w - 1) / 2;
  int doorY = y + h - 1;
  build(doorX, doorY, "door_open");
  openConnections.push_back({Level::Type::House, TilePos(*level, doorX, doorY)});
  overlay(x, y + h - 1 - depth, "brown_roof_angular_left_lower");
  overlay(x + w - 1, y + h - 1 - depth, "brown_roof_angular_right_lower");
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

float LevelGen::getHeight(int x, int y) {
  float w = level->getWidth();
  float h = level->getHeight();
  float result = 0.1f + stb_perlin_noise3(15 * x / w, 15 * y / h, 7);
  if (result > 1)
    return 1;
  return result;
}

void LevelGen::generate_overworld() {
  const int w = level->getWidth();
  const int h = level->getHeight();
  GameData& data = level->getData();

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float height = getHeight(x, y);
      if (height > 0) {
        level->get(x, y).setData(data.tile("grass"));
      } else {
        level->get(x, y).setData(data.tile("water_c"));
      }
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float vegetation =
          stb_perlin_noise3(12 * x / (float)w, 42, 12 * y / (float)h);
      float height = getHeight(x, y);
      if (height > 0) {
        if (vegetation > 0.3f && dis(gen) > 0.7f) {
          make_tree(x, y);
        } else if (vegetation > 0 && dis(gen) > 0.8f) {
          // make_bush(x, y);
        } else if (dis(gen) > 0.8f) {
          make_bush(x, y, dis(gen));
        }
      } else {
        if (height > -0.1f && vegetation > 0 && dis(gen) > 0.8f) {
          level->getBuilding(x, y).setData(data.tile("lilypad"));
        }
      }
    }
  }

  int tree_border = 8;

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float height = getHeight(x, y);

      int coordX = x;
      int coordY = y;
      if (w - x < tree_border)
        coordX = w - x;
      if (h - y < tree_border)
        coordY = h - y;
      for (float coord : {coordX, coordY}) {
        if (coordX > tree_border && coordY > tree_border)
          continue;
        float tree_chance = (tree_border - coord) / static_cast<float>(tree_border);
        if (height >= 0) {
          if (dis(gen) < tree_chance) {
            make_tree(x, y, true);
          }
        }

      }
    }
  }

  level->getBuilding(5, 5).setData(data.tile("fireplace"));

  make_house(48, 38, 6, 8, 3);
}

void LevelGen::generate_house() {
  const int w = level->getWidth();
  const int h = level->getHeight();
  GameData& data = level->getData();

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h - 1; ++y) {
      if (y == 0) {
        level->get(x, y).setData(data.tile("sand_wall_mid"));
        if (dis(gen) > 0.9f) {
          level->getOverlay(x, y).setData(data.tile("brown_window_round"));
        }
      } else if (y == 1) {
        level->get(x, y).setData(data.tile("sand_wall_lower_mid"));
        if (dis(gen) > 0.7f) {
          level->getOverlay(x, y).setData(data.tile("shelf"));
        }
      } else {
        level->get(x, y).setData(data.tile("planks"));
      }
    }
  }
  int doorX = (w - 1) / 2;
  int doorY = h - 1;
  openConnections.push_back({Level::Type::Overworld, TilePos(*level, doorX, doorY)});
  level->get(doorX, doorY).setData(data.tile("planks"));
  overlay(doorX, doorY, "door_light");
}

Level *LevelGen::generate(World& world, GameData &data, Level::Type type) {
  this->data = &data;

  switch(type) {
  case Level::Type::Overworld:
    level = new Level(world, type, 200, 200, data);
    generate_overworld();
    break;
  case Level::Type::House:
    level = new Level(world, type, 10, 10, data);
    generate_house();
    break;
  default:
    assert(false && "Not implemented level type for generation!");
  }
  world.addLevel(level);

  return level;
}
