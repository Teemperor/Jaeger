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

  void makeChest(int x, int y);

  void makeTree(int x, int y, bool force = false);
  void makeBush(int x, int y, float random);

  void makeFloor(TileRect A, std::string Prefix);

  void makeHouse(TileRect A, int depth,
                 Level::Type ConnectsTo = Level::Type::House);

  void makeMine(int x, int y);

  void generateSettlements();

  void floor(int x, int y, std::string tileName);
  void build(int x, int y, std::string tileName);
  void build2(int x, int y, std::string tileName);
  void overlay(int x, int y, std::string tileName);

  Perlin2D heightPerlin;
  Perlin2D woodPerlin;
  Perlin2D cavePerlin;

  float isCave(int x, int y) {
    float Result = getCavePerlin(x, y);
    // Depending on the distance from the center, the tile is less likely
    // to be a cave tile (further away are less cave tiles).
    int rx = x - level->getWidth() / 2;
    int ry = y - level->getHeight() / 2;
    float dx = std::abs(rx) / (float) (level->getWidth() / 2);
    float dy = std::abs(ry) / (float) (level->getHeight() / 2);
    Result *= 1 - std::sqrt(dx * dx + dy * dy);
    return Result > 0.2f;
  }
  float getCavePerlin(int x, int y);
  float getHeight(int x, int y);
  float getVegetation(int x, int y);
  std::mt19937 gen;
  std::uniform_real_distribution<float> dis;

  float chance() {
    return dis(gen);
  }

  void generateOverworld();

  bool generateSettlement(TileRect Area, int limit = 3);

  void generateHouse();

  void generateMine();

  void placeOrcCamp(int w, int h);

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
    enum class Directions {
      Up,
      Down,
      Left,
      Right
    };
    int TargetW = 0;
    int TargetH = 0;
    Directions Direction;
    Connection(Level::Type Target, TilePos P, Directions D = Directions::Up)
        : targetType(Target), pos(P), Direction(D) {
    }
    Connection() = default;
  };

private:
  std::vector<Connection> openConnections;
  Connection BackConnection;

public:
  explicit LevelGen(unsigned seed = 44);

  Level *generate(World &world, GameData &data, Level::Type type, const Connection *C);

  const std::vector<Connection> &getOpenConnections() const {
    return openConnections;
  }

  const Connection &getBackConnection() const {
    return BackConnection;
  }
};

#endif // ARGOS_LEVELGEN_H
