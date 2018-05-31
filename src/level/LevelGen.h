#ifndef ARGOS_LEVELGEN_H
#define ARGOS_LEVELGEN_H

#include "Level.h"
#include "TileRect.h"

#include <Perlin2D.h>
#include <random>

class World;

class LevelGen {
  GameData *data;
  Level *level;
  World *world;

  void make_chest(int x, int y);

  void make_tree(int x, int y, bool force = false);
  void make_bush(int x, int y, float random);

  void make_floor(TileRect A, std::string Prefix);

  void make_house(TileRect A, int depth,
                  Level::Type ConnectsTo = Level::Type::House);

  void make_mine(int x, int y);

  void generate_settlements();

  void floor(int x, int y, std::string tileName);
  void build(int x, int y, std::string tileName);
  void build2(int x, int y, std::string tileName);
  void overlay(int x, int y, std::string tileName);

  Perlin2D heightPerlin;
  Perlin2D woodPerlin;
  Perlin2D cavePerlin;

  float isCave(int x, int y) {
    return getCavePerlin(x, y) > 0.3f;
  }
  float getCavePerlin(int x, int y);
  float getHeight(int x, int y);
  float getVegetation(int x, int y);
  std::mt19937 gen;
  std::uniform_real_distribution<float> dis;

  float chance() {
    return dis(gen);
  }

  void generate_overworld();

  bool generate_settlement(TileRect Area, int limit = 3);

  void generate_house();

  void generate_mine();

  void makeStalagmite(int x, int y);

  enum TileCompare {
    CMP_S, // Same group
    CMP_D, // Different group
    CMP_A  // all groups
  };
  bool hasSurrounding(int x, int y, const std::string &group,
                      std::array<TileCompare, 8> surrounding);

  bool isFree(TileRect T) {
    for (int x = T.getX(); x <= T.getRightX(); ++x) {
      for (int y = T.getY(); y <= T.getLowerY(); ++y) {
        if (!level->getBuilding(x, y).empty())
          return false;
        if (!level->get(x, y).passable())
          return false;
      }
    }
    return true;
  }

public:
  struct Connection {
    Level::Type targetType;
    TilePos pos;
  };

private:
  std::vector<Connection> openConnections;

public:
  LevelGen(unsigned seed = 44);

  Level *generate(World &world, GameData &data, Level::Type type);

  const std::vector<Connection> &getOpenConnections() const {
    return openConnections;
  }
};

#endif // ARGOS_LEVELGEN_H
