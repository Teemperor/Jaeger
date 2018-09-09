#include "LevelGen.h"

#include "World.h"

#include <Character.h>
#include <set>

#include "stb_perlin.h"

template <typename T>
static int countSurroundingTiles(TileMap<T> &M, const int tx, const int ty,
                                 std::function<bool(const T &)> F) {
  int Count = 0;
  for (int x = tx - 1; x <= tx + 1; ++x) {
    for (int y = ty - 1; y <= ty + 1; ++y) {
      if (F(M.get(x, y)))
        ++Count;
    }
  }
  return Count;
}

void LevelGen::makeTree(int x, int y, bool force) {
  if (!force) {
    if (!level->getBuilding(x, y).empty())
      return;
    if (!level->getBuilding2(x, y).empty())
      return;
    if (level->get(x, y).group() != "grass")
      return;
  }
  std::string isDark;
  if (chance() > 0.5f)
    isDark = "dark";

  if (chance() > 0.1f) {
    if (chance() > 0.5f) {
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

void LevelGen::makeBush(int x, int y, float random) {
  std::string tileName = "small_bush";
  if (random > 0.8f)
    tileName = "flower";
  if (!level->getBuilding(x, y).empty())
    return;
  if (level->get(x, y).group() != "grass")
    return;
  level->getBuilding(x, y).setData(data->tile(tileName));
}

void LevelGen::makeHouse(TileRect A, int depth, Level::Type ConnectsTo) {
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
        if (chance() > 0.0f) {
          overlay(ix, iy, "brown_window_round");
        }
      }
    }
  }
  int doorX = x + (w - 1) / 2;
  int doorY = y + h - 1;
  floor(doorX, doorY, "sand_wall_lower_mid_free");
  build(doorX, doorY, "door_open");

  openConnections.emplace_back(ConnectsTo, TilePos(*level, doorX, doorY));
  openConnections.back().TargetW = w * 2;
  openConnections.back().TargetH = h * 2;

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

  result += Elevation;
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

void LevelGen::generateWaterAndLand() {

  const int w = level->getWidth();
  const int h = level->getHeight();
  GameData &data = level->getData();

  TileMap<int> water(w, h, 0);

  for (int x = 0; x < w; ++x) {
      for (int y = 0; y < h; ++y) {
          float height = getHeight(x, y);
          level->get(x, y).setData(data.tile("grass"));
          if (height <= 0) {
              water.get(x, y) = 1;
            }
        }
    }

  bool Changed = true;
  while (Changed) {
      Changed = false;
      for (int x = 0; x < w; ++x) {
          for (int y = 0; y < h; ++y) {
              if (water.get(x, y) == 0)
                continue;
              int SurroundingGrass = countSurroundingTiles<int>(
                                       water, x, y, [](const int &T) { return T == 0; });
              if (SurroundingGrass >= 5) {
                  Changed = true;
                  water.get(x, y) = 0;
                }
            }
        }
    }

  for (int x = 0; x < w; ++x) {
      for (int y = 0; y < h; ++y) {
          if (water.get(x, y)) {
              if (chance() > 0.01f)
                level->get(x, y).setData(data.tile("water_c"));
              else
                floor(x, y, "water_rock");
            }
        }
    }
}

void LevelGen::formatWaterTiles() {
  int w = level->getWidth();
  int h = level->getHeight();

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
}

void LevelGen::placeVegetation() {
  auto w = level->getWidth();
  auto h = level->getHeight();
  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      float vegetation = getVegetation(x, y);
      float height = getHeight(x, y);
      if (height > 0) {
        if (vegetation > 0.2f && chance() > 0.75f) {
          makeTree(x, y);
        } else if (vegetation > 0 && chance() > 0.8f) {
          // make_bush(x, y);
        } else if (chance() > 0.8f) {
          makeBush(x, y, chance());
        }
      } else {
        if (height > -0.1f && vegetation > 0 && chance() > 0.8f) {
          level->getBuilding(x, y).setData(data->tile("lilypad"));
        }
      }
    }
  }
}

void LevelGen::placeTreeBorder() {
  auto w = level->getWidth();
  auto h = level->getHeight();

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
          if (chance() < tree_chance) {
            makeTree(x, y);
          }
        }
      }
    }
  }
}

namespace {
  class Area {
  public:
    int x1, y1, x2, y2;
  Area(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

  void foreach (std::function<void(int, int)> F) {
    for (int x = x1; x < x2; x++) {
      for (int y = y1; y < y2; y++) {
        F(x, y);
      }
    }
  }

  bool contains(int x, int y) {
    return x >= x1 && x <= x2 && y >= y1 && y <= y2;
  }

  bool overlaps(Area &a) {
    return (this->x1 < a.x2 && this->x2 > a.x1 && this->y1 > a.y2 &&
            this->y2 < a.y1);
  }
};
} // namespace

void LevelGen::generateOverworld() {

  generateWaterAndLand();

  if (chance() < 0.5f) {
    for (int i = 0; i < tree_border; i++) {
      build(49, i, "placeholder");
    }
    openConnections.emplace_back(Level::Type::Overworld,
                                 TilePos(*level, 49, 3));
  }

  int h = level->getHeight();
  for (int i = 0; i < tree_border; i++) {
    build(49, h - i, "placeholder");
  }
  BackConnection =
      Connection(Level::Type::Overworld, TilePos(*level, 49, h - 3));

  formatWaterTiles();

  generateSettlements();

  for (int i = 0; i < chance() * 5; ++i) {
    makeMine(chanceInt(level->getWidth()), chanceInt(level->getHeight()));
  }

  int camps = 0;
  for (int i = 0; i < chance() * 25 && camps < 3; ++i) {
    if (makeCamp())
      camps++;
  }

  placeTreeBorder();
  placeVegetation();
}

bool LevelGen::makeCamp() {
  TileRect A(chanceInt(level->getWidth()), chanceInt(level->getHeight()), 4, 4);
  if (!isFree(A))
    return false;
  int x = A.getX();
  int y = A.getY();

  build2(x, y, "tent_upper_left_green");
  build2(x + 1, y, "tent_upper_right_green");
  build2(x, y + 1, "tent_lower_left_green");
  build2(x + 1, y + 1, "tent_lower_right_green");

  if (chance() < 0.3f)
    build2(x + 1, y + 3, "fireplace");
  else if (chance() < 0.5f)
    build2(x, y + 3, "fireplace");

  if (chance() < 0.3f)
    build2(x, y + 3, "wood_stump_axe");

  if (chance() < 0.3f) {
    build2(x + 2, y + 1, "wood_chest");
  } else if (chance() < 0.5f) {
    build2(x + 2, y + 1, "wood_stack");
  }

  build(x, y + 1, "earth_tl");
  build(x, y + 2, "earth_bl");
  build(x + 1, y + 1, "earth_tr");
  build(x + 1, y + 2, "earth_br");

  return true;
}

void LevelGen::generateHouse() {
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

    if (chance() > 0.8f)
      build(doorX - 1, h - 3, "pot_plant");
    if (chance() > 0.8f)
      build(doorX + 1, h - 3, "pot_plant");

    if (x && x != w - 1) {
      if (chance() > 0.9f)
        build(x, 1, "brown_window_round");
      if (chance() > 0.7f)
        build(x, 2, "shelf");
    }
  }
  makeChest(doorX + 2, h - 3);

  for (int y = 1; y < h - 2; ++y) {
    overlay(0, y, "sand_walltop_vertical");
    overlay(w - 1, y, "sand_walltop_vertical");

    if (chance() > 0.8f)
      build(w - 2, y, "torch_wall_right");
    if (chance() > 0.7f)
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
      if (chance() > 0.999f)
        build(x, y, "wood_table");
    }
  }

  int doorY = h - 1;
  BackConnection =
      Connection(Level::Type::Overworld, TilePos(*level, doorX, doorY));
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
    : heightPerlin(seed, 0.07), woodPerlin(seed * 55, 0.03),
      cavePerlin(seed * 3, 0.07), gen(seed), dis(0, 1) {
  Elevation = chance() * 3;
}

Level *LevelGen::generate(World &world, GameData &data, Level::Type type,
                          const Connection *C) {
  this->world = &world;
  this->data = &data;

  level = nullptr;
  switch (type) {
  case Level::Type::Overworld:
    level = new Level(world, type, 200, 200, data);
    generateOverworld();
    break;
  case Level::Type::House:
    assert(C);
    level = new Level(world, type, C->TargetW, C->TargetH, data);
    generateHouse();
    break;
  case Level::Type::Mine:
    level = new Level(world, type, 50 + chanceInt(100),
                      50 + chanceInt(100), data);
    generateMine();
    break;
  }
  assert(level && "Not implemented level type for generation!");
  world.addLevel(level);

  return level;
}

bool LevelGen::generateSettlementPiece(TileRect Area, int iteration) {
  if (!isFree(Area))
    return false;

  if (iteration >= 6)
    return false;

  if (iteration == 0) {
    makeFloor(Area, "stone");
    decorateMarketplace(Area);
  } else if (iteration < 4 && Area.biggerThan(3, 4) && !Area.biggerThan(6, 6)) {
    auto UsedArea = Area.resize(-1, -1);
    makeHouse(UsedArea, 2);
    makeLine(TilePos(UsedArea.getX(), UsedArea.getY() + UsedArea.getH()),
             TilePos(UsedArea.getX() + UsedArea.getW(),
                     UsedArea.getY() + UsedArea.getH()),
             "grass_stones");
  } else if (iteration > 4) {
    auto UsedArea = Area.resize(-1, -1);
    makeFloor(UsedArea, "earth");

    UsedArea = UsedArea.resize(0, -1);
    for (int x = UsedArea.getX(); x <= UsedArea.getRightX(); ++x) {
      for (int y = UsedArea.getY(); y <= UsedArea.getLowerY(); ++y) {
        build2(x, y, "corn");
      }
    }
  }

  iteration++;

  std::vector<std::pair<int, int>> Offsets = {
      std::make_pair(-1, 0),
      std::make_pair(1, 0),
      std::make_pair(0, 1),
      std::make_pair(0, -1),
  };

  for (auto O : Offsets) {
    if (chance() < 0.2f)
      continue;
    for (int Try = 0; Try < 5; ++Try) {
      int NewW = static_cast<int>(chanceInt(2) + 4);
      int NewH = static_cast<int>(chanceInt(2) + 5);
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

      if (generateSettlementPiece(New, iteration))
        break;
    }
  }
  return true;
}

void LevelGen::generateSettlements() {
  int Count = 3;
  for (int i = 0; i < 4000; i++) {
    if (Count == 0)
      break;
    int x = chanceInt(level->getWidth());
    int y = chanceInt(level->getHeight());
    int w = 7 + chanceInt(4);
    int h = 7 + chanceInt(4);

    if (generateSettlementPiece(TileRect(x, y, w, h))) {
      --Count;
    }
  }
}

void LevelGen::makeFloor(TileRect A, std::string Prefix) {
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

bool LevelGen::makeStall(int x, int y) {
  for (int ix = x; ix < x + 3; ++ix)
    for (int iy = y - 1; iy < y + 4; ++iy) {
      if (!level->getBuilding(ix, iy).empty())
        return false;
    }

  build(x, y, "stall_table_left");
  build(x + 1, y, "stall_table_mid");
  build(x + 2, y, "stall_table_right");

  std::string color = "green";
  if (chance() < 0.5f)
    color = "red";

  overlay(x, y - 1, "stall_cover_left_" + color);
  overlay(x + 1, y - 1, "stall_cover_mid_" + color);
  overlay(x + 2, y - 1, "stall_cover_right_" + color);
  return true;
}

void LevelGen::decorateMarketplace(TileRect A) {
  A = A.moveX(1);
  A = A.moveY(2);
  A = A.resize(-2, -2);

  unsigned placed = 0;
  for (int i = 0; i < 1000 && placed < 5; ++i) {
    if (makeStall(A.getX() + chanceInt(A.getW()),
                  A.getY() + chanceInt(A.getH())))
      ++placed;

  }
}

void LevelGen::makeLine(TilePos Start, TilePos End, std::string T) {
  if (Start.getX() == End.getX()) {
    for (int y = std::min(Start.getY(), End.getY());
         y <= std::max(Start.getY(), End.getY()); ++y) {
      build(Start.getX(), y, T);
    }
  } else if (Start.getY() == End.getY()) {
    for (int x = std::min(Start.getX(), End.getX());
         x <= std::max(Start.getX(), End.getX()); ++x) {
      build(x, Start.getY(), T);
    }
  } else {
    assert(false && "Neither same x nor y coords for makeLine");
  }
}

void LevelGen::makeMine(int x, int y) {
  TileRect Area(x - 1, y - 3, 3, 9);
  if (!isFree(Area))
    return;
  makeFloor(Area.resize(-1, -4).moveY(1), "earth");
  makeHouse(Area.resize(0, -6), 1, Level::Type::Mine);
  build(x, y, "railtracks_vertical");
  build(x, y + 1, "railtracks_vertical");
  build(x, y + 1, "railtracks_end_bottom");
}

void LevelGen::generateMine() {
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
        TilePos(x, y + 1),     TilePos(x, y - 1),     TilePos(x + 1, y),
        TilePos(x - 1, y),     TilePos(x + 1, y + 1), TilePos(x - 1, y + 1),
        TilePos(x + 1, y - 1), TilePos(x - 1, y - 1),
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

      bool LeftFloor = level->get(x - 1, y).name() == "cave_floor" &&
                       level->getBuilding(x - 1, y).empty();
      bool RightFloor = level->get(x + 1, y).name() == "cave_floor" &&
                        level->getBuilding(x + 1, y).empty();
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

  BackConnection =
      Connection(Level::Type::Overworld, TilePos(*level, DoorX, DoorY));
  level->get(DoorX, DoorY).setData(data.tile("cave_floor"));
  overlay(DoorX, DoorY, "door_light");

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (x == DoorX && y == DoorY)
        continue;
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
      if (x == DoorX && y == DoorY)
        continue;
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
      if (TilePos(x, y).distance(TilePos(DoorX, DoorY)) < 6)
        continue;
      if (x == DoorX && y == DoorY)
        continue;
      if (level->get(x, y).name() != "cave_floor")
        continue;
      if (level->get(x, y - 1).name() != "cave_floor")
        continue;
      if (level->get(x, y + 1).name() != "cave_floor")
        continue;
      if (chance() < 0.03f)
        makeStalagmite(x, y);
    }
  }

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      if (x == DoorX && y == DoorY)
        continue;
      if (level->get(x, y).name() != "cave_floor")
        continue;
      if (!level->get(x, y + 1).empty())
        continue;
      build(x, y, "cave_wall_top");
      build(x, y + 1, "cave_wall_single");
    }
  }

  placeOrcCamp(w, h);
}

void LevelGen::makeChest(int x, int y) {
  build(x, y, "chest");
  level->getBuilding(x, y).getInventory()->add(
      Item(*data->item("small_health_potion"), *world));
}

void LevelGen::makeStalagmite(int x, int y) {
  if (chance() < 0.4f)
    build(x, y, "stalagmite_small");
  else {
    overlay(x, y - 1, "stalagmite_top");
    build(x, y, "stalagmite_bottom");
  }
}

void LevelGen::placeOrcCamp(int w, int h) {

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {

      bool GoodPlace = true;
      Area A(x - 1, y - 1, x + 2, y + 2);
      A.foreach ([&GoodPlace, this](int ix, int iy) {
        if (!level->passable(TilePos(ix, iy)))
          GoodPlace = false;
      });
      if (!GoodPlace)
        continue;

      build(x, y, "fireplace");

      Faction *F = new Faction();

      Character *C;
      C = new Character(*level, Vec2(TilePos(x - 1, y)),
                        Character::BodyType::Green);
      C->setFaction(F);
      C = new Character(*level, Vec2(TilePos(x + 1, y)),
                        Character::BodyType::Green);
      C->setFaction(F);
      C = new Character(*level, Vec2(TilePos(x + 1, y + 1)),
                        Character::BodyType::Green);
      C->setFaction(F);
      return;
    }
  }
}
