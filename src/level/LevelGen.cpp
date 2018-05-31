#include "LevelGen.h"

#include "World.h"

#include <set>

#include "stb_perlin.h"

void LevelGen::make_tree(int x, int y, bool force) {
  if (!force) {
    if (!level->getBuilding(x, y).empty())
      return;
  }
  std::string isDark;
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

void LevelGen::make_house(TileRect A, int depth, Level::Type ConnectsTo) {
  int x = A.getX();
  int y = A.getY();
  int w = A.getW();
  int h = A.getH();
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

  openConnections.push_back({ConnectsTo, TilePos(*level, doorX, doorY)});

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

void LevelGen::build2(int x, int y, std::string tileName) {
  level->getBuilding2(x, y).setData(data->tile(tileName));
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

float LevelGen::getCavePerlin(int x, int y) {
  float result = cavePerlin.get(x, y);

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
  Area(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

  bool contains(int x, int y) {
    return x >= x1 && x <= x2 && y >= y1 && y <= y2;
  }

  bool overlaps(Area &a) {
    return (this->x1 < a.x2 && this->x2 > a.x1 && this->y1 > a.y2 &&
            this->y2 < a.y1);
  }
};
} // namespace

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

  generate_settlements();

  make_mine(50, 50);

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
  make_chest(doorX + 2, h - 3);

  for (int y = 1; y < h - 2; ++y) {
    overlay(0, y, "sand_walltop_vertical");
    overlay(w - 1, y, "sand_walltop_vertical");

    if (dis(gen) > 0.8f)
      build(w - 2, y, "torch_wall_right");
    if (dis(gen) > 0.7f)
      build(1, y, "torch_wall_left");
  }
  overlay(0, 0, "sand_walltop_leftup");
  overlay(w - 1, 0, "sand_walltop_rightup");
  overlay(0, h - 2, "sand_walltop_leftbottom");
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
    : heightPerlin(seed, 0.07), woodPerlin(seed * 55, 0.03), cavePerlin(seed * 3, 0.07), gen(seed),
      dis(0, 1) {}

Level *LevelGen::generate(World &world, GameData &data, Level::Type type) {
  this->world = &world;
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
  case Level::Type::Mine:
    level = new Level(world, type, 100, 100, data);
    generate_mine();
    break;
  default:
    assert(false && "Not implemented level type for generation!");
  }
  world.addLevel(level);

  return level;
}

bool LevelGen::generate_settlement(TileRect Area, int limit) {
  if (!isFree(Area))
    return false;

  if (limit <= 0)
    return false;

  auto UsedArea = Area.resize(-1, -1);
  if (Area.biggerThan(3, 4))
    make_house(UsedArea, 2);
  else {
    make_floor(UsedArea, "earth");

    UsedArea = UsedArea.resize(0, -1);
    for (int x = UsedArea.getX(); x <= UsedArea.getRightX(); ++x) {
      for (int y = UsedArea.getY(); y <= UsedArea.getLowerY(); ++y) {
        build2(x, y, "corn");
      }
    }
  }

  --limit;

  std::vector<std::pair<int, int>> Offsets = {
      std::make_pair(-1, 0),
      std::make_pair(1, 0),
      std::make_pair(0, 1),
      std::make_pair(0, -1),
  };

  for (auto O : Offsets) {
    if (dis(gen) < 0.6f)
      continue;
    for (int Try = 0; Try < 5; ++Try) {
      int NewW = static_cast<int>(dis(gen) * 4 + 3);
      int NewH = static_cast<int>(dis(gen) * 4 + 3);
      TileRect New = Area;
      New.setW(NewW);
      New.setH(NewH);

      if (O.first == -1)
        New = New.moveX(-NewW - 1);
      else if (O.first == 1)
        New = New.moveX(Area.getW() + 1);

      if (O.second == -1)
        New = New.moveY(-NewH - 1);
      else if (O.second == 1)
        New = New.moveY(Area.getH() + 1);

      if (generate_settlement(New, limit))
        break;
    }
  }
  return true;
}

void LevelGen::generate_settlements() {
  int Count = 15;
  for (int i = 0; i < 4000; i++) {
    if (Count == 0)
      break;
    int x = static_cast<int>(dis(gen) * level->getWidth());
    int y = static_cast<int>(dis(gen) * level->getHeight());
    int w = 7;
    int h = 7;

    if (generate_settlement(TileRect(x, y, w, h))) {
      --Count;
    }
  }
}

void LevelGen::make_floor(TileRect A, std::string Prefix) {
  Prefix += "_";

  build(A.getX(), A.getY(), Prefix + "tl");
  build(A.getX(), A.getLowerY(), Prefix + "bl");
  build(A.getRightX(), A.getY(), Prefix + "tr");
  build(A.getRightX(), A.getLowerY(), Prefix + "br");

  for (int x = A.getX() + 1; x < A.getRightX(); ++x) {
    build(x, A.getY(), Prefix + "t");
    build(x, A.getLowerY(), Prefix + "b");
  }
  for (int y = A.getY() + 1; y < A.getLowerY(); ++y) {
    build(A.getX(), y, Prefix + "l");
    build(A.getRightX(), y, Prefix + "r");
  }

  for (int x = A.getX() + 1; x < A.getRightX(); ++x) {
    for (int y = A.getY() + 1; y < A.getLowerY(); ++y) {
      floor(x, y, Prefix + "c");
    }
  }
}

void LevelGen::make_mine(int x, int y) {
  TileRect Area(x - 1, y - 3, 3, 9);
  if (!isFree(Area))
    return;
  make_floor(Area.resize(-1, -4).moveY(1), "earth");
  make_house(Area.resize(0, -6), 2, Level::Type::Mine);
  build(x, y, "railtracks_vertical");
  build(x, y + 1, "railtracks_vertical");
  build(x, y + 1, "railtracks_end_bottom");
}

void LevelGen::generate_mine() {
  const int w = level->getWidth();
  const int h = level->getHeight();
  GameData &data = level->getData();

  int DoorX = w / 2;

  int StartX = w / 2;
  int StartY = h / 2;
  while (!isCave(StartX, StartY)) {
    StartY += 1;
    assert(StartY <= h);
  }

  int DoorY = StartY;
  while (isCave(DoorX, DoorY)) {
    DoorY += 1;
    assert(DoorY <= h);
  }
  --DoorY;


  std::set<TilePos> Checked = {TilePos(StartX, StartY)};
  std::vector<TilePos> ToCheck = {TilePos(StartX, StartY)};

  while (!ToCheck.empty()) {
    TilePos P = ToCheck.back();
    ToCheck.pop_back();
    Checked.insert(P);

    int x = P.getX();
    int y = P.getY();
    if (isCave(x, y))
      floor(x, y, "cave_floor");
    else
      continue;

    std::vector<TilePos> ToAdd = {
        TilePos(x, y + 1),
        TilePos(x, y - 1),
        TilePos(x + 1, y),
        TilePos(x - 1, y),
        TilePos(x + 1, y + 1),
        TilePos(x - 1, y + 1),
        TilePos(x + 1, y - 1),
        TilePos(x - 1, y - 1),
    };
    for (TilePos T : ToAdd) {
      if (x < 0 || y < 0 || x >= w || y >= h)
        continue;
      if (Checked.find(T) == Checked.end()) {
        Checked.insert(T);
        ToCheck.push_back(T);
      }
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (!level->get(x, y).empty())
        continue;
      if (level->get(x, y + 1).name() != "cave_floor")
        continue;
      floor(x, y, "cave_floor");

      bool LeftFloor = level->get(x - 1, y).name() == "cave_floor"
                      && level->getBuilding(x - 1, y).empty();
      bool RightFloor = level->get(x + 1, y).name() == "cave_floor"
                        && level->getBuilding(x + 1, y).empty();
      std::string Suffix;
      if (LeftFloor && RightFloor)
        Suffix = "_single";
      else if (LeftFloor)
        Suffix = "_left";
      else if (RightFloor)
        Suffix = "_right";
      else
        Suffix = "";

      build(x, y, "cave_wall_bottom" + Suffix);
      build(x, y - 1, "cave_wall" + Suffix);
      build(x, y - 2, "cave_wall_top");
    }
  }

  openConnections.push_back(
      {Level::Type::Overworld, TilePos(*level, DoorX, DoorY)});
  level->get(DoorX, DoorY).setData(data.tile("cave_floor"));
  overlay(DoorX, DoorY, "door_light");

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (x == DoorX && y == DoorY) continue;
      if (!level->get(x, y).empty())
        continue;
      if (!level->get(x, y + 1).empty())
        continue;
      if (level->get(x + 1, y).name() != "cave_floor")
        continue;
      build(x, y, "cave_wall_right");
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (x == DoorX && y == DoorY) continue;
      if (!level->get(x, y).empty())
        continue;
      if (!level->get(x, y + 1).empty())
        continue;
      if (level->get(x - 1, y).name() != "cave_floor")
        continue;
      build(x, y, "cave_wall_left");
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (x == DoorX && y == DoorY) continue;
      if (level->get(x, y).name() != "cave_floor")
        continue;
      if (!level->get(x, y + 1).empty())
        continue;
      build(x, y, "cave_wall_top");
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (TilePos(x, y).distance(TilePos(DoorX, DoorY)) < 6)
        continue;
      if (x == DoorX && y == DoorY) continue;
      if (level->get(x, y).name() != "cave_floor")
        continue;
      if (level->get(x, y - 1).name() != "cave_floor")
        continue;
      if (chance() < 0.03f)
        makeStalagmite(x, y);
    }
  }

}

void LevelGen::make_chest(int x, int y) {
  build(x, y, "chest");
  level->getBuilding(x, y).getInventory()->add(Item(*data->item("small_health_potion"), *world));
}

void LevelGen::makeStalagmite(int x, int y) {
  if (chance() < 0.4)
    build(x, y, "stalagmite_small");
  else {
    build(x, y - 1, "stalagmite_top");
    build(x, y, "stalagmite_bottom");
  }
}
