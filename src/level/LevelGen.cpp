#include "LevelGen.h"

#include "World.h"

#include <set>

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
      level->getBuilding(x, y).setData(
          data->tile("tree_" + isDark + "green_lower"));
      level->getOverlay(x, y - 1).setData(
          data->tile("tree_" + isDark + "green_upper"));
    } else {
      level->getBuilding(x, y).setData(
          data->tile("pine_" + isDark + "green_lower"));
      level->getOverlay(x, y - 1).setData(
          data->tile("pine_" + isDark + "green_upper"));
    }
  } else {
    level->getBuilding(x, y).setData(
        data->tile("tree_" + isDark + "green_small"));
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
  for (int ix = x; ix < x + w; ix++) {
    for (int iy = y + 1; iy < y + h; iy++) {
      if (ix == x) {
        build(ix, iy, "sand_wall_left");
      } else if (ix == x + w - 1) {
        build(ix, iy, "sand_wall_right");
      } else if (iy == y + h - 1) {
        floor(ix, iy, "sand_wall_lower_mid");
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
          overlay(ix, iy, "brown_window_round");
        }
      }
    }
  }
  int doorX = x + (w - 1) / 2;
  int doorY = y + h - 1;
  floor(doorX, doorY, "sand_wall_lower_mid_free");
  build(doorX, doorY, "door_open");
  openConnections.push_back(
      {Level::Type::House, TilePos(*level, doorX, doorY)});

  overlay(x, y + h - 1 - depth, "brown_roof_angular_left_lower");
  overlay(x + w - 1, y + h - 1 - depth, "brown_roof_angular_right_lower");
  overlay(x, y, "brown_roof_angular_left_upper");
  overlay(x + w - 1, y, "brown_roof_angular_right_upper");
  build(x, y + h - 1, "sand_wall_lower_left");
  build(x + w - 1, y + h - 1, "sand_wall_lower_right");
}

void LevelGen::floor(int x, int y, std::string tileName) {
  level->get(x, y).setData(data->tile(tileName));
}

void LevelGen::build(int x, int y, std::string tileName) {
  level->getBuilding(x, y).setData(data->tile(tileName));
}

void LevelGen::overlay(int x, int y, std::string tileName) {
  level->getOverlay(x, y).setData(data->tile(tileName));
}

float LevelGen::getHeight(int x, int y) {
  float result = heightPerlin.get(x, y);

  // Coast:
  // result += 0.5f;
  // result += - 1.5f * (x / (float) level->getWidth() * 0.8f);

  /* Archipelago:
  Vec2 mid(level->getWidth()/2, level->getHeight()/2);
  float dist = mid.distance(Vec2(x, y));
  dist /= level->getWidth() / 2;
  result *= (1 - dist);

  result -= 0.1f; */

  result += 0.3f;
  if (result > 1)
    return 1;
  return result;
}

float LevelGen::getVegetation(int x, int y) { return woodPerlin.get(x, y); }

namespace {
class Area {
public:
  int x1, y1, x2, y2;
  Area(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {
  }

  bool contains(int x, int y) {
    return x >= x1 && x <= x2 && y >= y1 && y <= y2;
  }

  bool overlaps(Area& a) {
    return (this->x1 < a.x2 && this->x2 > a.x1 &&
        this->y1 > a.y2 && this->y2 < a.y1);
  }

};
}

void LevelGen::generate_overworld() {
  const int w = level->getWidth();
  const int h = level->getHeight();
  GameData &data = level->getData();

  const int tree_border = 8;

  std::vector<Area> areas;

  TileMap<int> water(w, h, 0);

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float height = getHeight(x, y);
      if (height > 0) {
        level->get(x, y).setData(data.tile("grass"));
      } else {
        water.get(x, y) = 1;
      }
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (water.get(x, y)) {
        if (dis(gen) > 0.01f)
          level->get(x, y).setData(data.tile("water_c"));
        else
          floor(x, y, "water_rock");
      }
    }
  }
  for (int i = 0; i < tree_border; i++) {
    build(49, i, "placeholder");
    build(50, i, "placeholder");
  }



  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (level->get(x, y).group() == "water") {
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_D, CMP_A, CMP_S, CMP_S, CMP_A, CMP_S, CMP_A}))
          floor(x, y, "water_outer_up");
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_S, CMP_A, CMP_S, CMP_S, CMP_A, CMP_D, CMP_A}))
          floor(x, y, "water_outer_down");
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_S, CMP_A, CMP_D, CMP_S, CMP_A, CMP_S, CMP_A}))
          floor(x, y, "water_outer_left");
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_S, CMP_A, CMP_S, CMP_D, CMP_A, CMP_S, CMP_A}))
          floor(x, y, "water_outer_right");

        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_D, CMP_A, CMP_D, CMP_S, CMP_A, CMP_S, CMP_A}))
          floor(x, y, "water_outer_up_left");
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_D, CMP_A, CMP_S, CMP_D, CMP_A, CMP_S, CMP_A}))
          floor(x, y, "water_outer_up_right");
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_S, CMP_A, CMP_D, CMP_S, CMP_A, CMP_D, CMP_A}))
          floor(x, y, "water_outer_down_left");
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_S, CMP_A, CMP_S, CMP_D, CMP_A, CMP_D, CMP_A}))
          floor(x, y, "water_outer_down_right");

        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_S, CMP_A, CMP_S, CMP_S, CMP_A, CMP_S, CMP_D}))
          floor(x, y, "water_inner_up_left");
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_S, CMP_A, CMP_S, CMP_S, CMP_D, CMP_S, CMP_S}))
          floor(x, y, "water_inner_up_right");
        if (hasSurrounding(
                x, y, "water",
                {CMP_D, CMP_S, CMP_A, CMP_S, CMP_S, CMP_A, CMP_S, CMP_S}))
          floor(x, y, "water_inner_down_right");
        if (hasSurrounding(
                x, y, "water",
                {CMP_A, CMP_S, CMP_D, CMP_S, CMP_S, CMP_A, CMP_S, CMP_S}))
          floor(x, y, "water_inner_down_left");
      }
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float vegetation = getVegetation(x, y);
      float height = getHeight(x, y);
      if (height > 0) {
        if (vegetation > 0.2f && dis(gen) > 0.75f) {
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

  for(int i = 0; i < 300; i++) {
    int x1 = static_cast<int>(dis(gen) * w);
    int y1 = static_cast<int>(dis(gen) * h);
    int x2 = static_cast<int>(x1 + dis(gen) * 20 + 7);
    int y2 = static_cast<int>(y1 + dis(gen) * 30 + 7);

    bool isFree = true;
    for (int x = x1; x <= x2; ++x) {
      for (int y = y1; y <= y2; ++y) {
        if (!level->passable(TilePos(x, y))) {
          isFree = false;
          x = x2 + 1;
          break;
        }
      }
    }
    if (!isFree)
      continue;

    make_house(x1, y1, 7, 7, 3);
  }

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
        float tree_chance =
            (tree_border - coord) / static_cast<float>(tree_border);
        if (height >= 0) {
          if (dis(gen) < tree_chance) {
            make_tree(x, y);
          }
        }
      }
    }
  }
}

void LevelGen::generate_house() {
  const int w = level->getWidth();
  const int h = level->getHeight();
  GameData &data = level->getData();

  int doorX = (w - 1) / 2;

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h - 1; ++y) {
      level->get(x, y).setData(data.tile("planks"));
    }
  }

  for (int x = 0; x < w; ++x) {
    overlay(x, 0, "sand_walltop_horizontal");
    floor(x, 1, "sand_wall_mid");
    floor(x, 2, "sand_wall_lower_mid");

    if (x != doorX) {
      if (x == doorX - 1) {
        overlay(x, h - 2, "sand_walltop_leftend");
        overlay(x, h - 1, "sand_wall_right");
      } else if (x == doorX + 1) {
        overlay(x, h - 2, "sand_walltop_rightend");
        overlay(x, h - 1, "sand_wall_left");
      } else {
        overlay(x, h - 2, "sand_walltop_horizontal");
        overlay(x, h - 1, "sand_wall_mid");
      }
    }

    if (dis(gen) > 0.8f)
      build(doorX - 1, h - 3, "pot_plant");
    if (dis(gen) > 0.8f)
      build(doorX + 1, h - 3, "pot_plant");

    if (x && x != w - 1) {
      if (dis(gen) > 0.9f)
        build(x, 1, "brown_window_round");
      if (dis(gen) > 0.7f)
        build(x, 2, "shelf");
    }
  }

  for (int y = 1; y < h - 2; ++y) {
    overlay(0, y, "sand_walltop_vertical");
    overlay(w - 1, y, "sand_walltop_vertical");

    if (dis(gen) > 0.8f)
      build(w - 2, y, "torch_wall_right");
    if (dis(gen) > 0.7f)
      build(1, y, "torch_wall_left");
  }
  overlay(    0,     0, "sand_walltop_leftup");
  overlay(w - 1,     0, "sand_walltop_rightup");
  overlay(    0, h - 2, "sand_walltop_leftbottom");
  overlay(w - 1, h - 2, "sand_walltop_rightbottom");

  build((w - 1) / 2, 1, "clock");
  build((w - 1) / 2, 2, "fireplace_inside");


  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h - 1; ++y) {
      if (!level->passable(TilePos(x, y)))
        continue;
      if (dis(gen) > 0.999f)
        build(x, y, "wood_table");
    }
  }


  int doorY = h - 1;
  openConnections.push_back(
      {Level::Type::Overworld, TilePos(*level, doorX, doorY)});
  level->get(doorX, doorY).setData(data.tile("planks"));
  overlay(doorX, doorY, "door_light");
}

bool LevelGen::hasSurrounding(
    int x, int y, const std::string &group,
    std::array<LevelGen::TileCompare, 8> surrounding) {
  std::vector<std::pair<int, int>> offsets = {
      {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
  for (unsigned int i = 0; i < offsets.size(); i++) {
    auto off = offsets[i];
    bool same = level->get(x + off.first, y + off.second).group() == group;
    switch (surrounding.at(i)) {
    case TileCompare::CMP_S:
      if (!same)
        return false;
      break;
    case TileCompare::CMP_D:
      if (same)
        return false;
      break;
    case TileCompare::CMP_A:
      break;
    }
  }
  return true;
}

LevelGen::LevelGen(unsigned seed)
    : heightPerlin(seed, 0.07), woodPerlin(seed * 55, 0.03), gen(seed),
      dis(0, 1) {}

Level *LevelGen::generate(World &world, GameData &data, Level::Type type) {
  this->data = &data;

  switch (type) {
  case Level::Type::Overworld:
    level = new Level(world, type, 200, 200, data);
    generate_overworld();
    break;
  case Level::Type::House:
    level = new Level(world, type, 12, 15, data);
    generate_house();
    break;
  default:
    assert(false && "Not implemented level type for generation!");
  }
  world.addLevel(level);

  return level;
}
