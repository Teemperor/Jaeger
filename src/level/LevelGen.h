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

  float Elevation = 0.3f;
  const int tree_border = 8;

  void makeChest(int x, int y);

  void makeTree(int x, int y, bool force = false);
  void makeBush(int x, int y, float random);

  void makeQuadLines(TileRect A, std::string T) {
    makeLine(TilePos(A.getX(), A.getY()),
             TilePos(A.getX(), A.getY() + A.getH()), T);
    makeLine(TilePos(A.getX() + A.getW(), A.getY()),
             TilePos(A.getX() + A.getW(), A.getY() + A.getH()), T);

    makeLine(TilePos(A.getX(), A.getY()),
             TilePos(A.getX() + A.getW(), A.getY()), T);
    makeLine(TilePos(A.getX(), A.getY() + A.getH()),
             TilePos(A.getX() + A.getW(), A.getY() + A.getH()), T);
  }
  void makeLine(TilePos Start, TilePos End, std::string T);

  void makeFloor(TileRect A, std::string Prefix);
  bool makeStall(int x, int y);
  void decorateMarketplace(TileRect A);

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

  bool isCave(int x, int y) {
    float Result = getCavePerlin(x, y);
    // Depending on the distance from the center, the tile is less likely
    // to be a cave tile (further away are less cave tiles).
    int rx = x - level->getWidth() / 2;
    int ry = y - level->getHeight() / 2;
    float dx = std::abs(rx) / static_cast<float>(level->getWidth() / 2);
    float dy = std::abs(ry) / static_cast<float>(level->getHeight() / 2);
    Result *= 1 - std::sqrt(dx * dx + dy * dy);
    return Result > 0.2f;
  }
  float getCavePerlin(int x, int y);
  float getHeight(int x, int y);
  float getVegetation(int x, int y);
  std::mt19937 gen;
  std::uniform_real_distribution<float> dis;

  template<typename T>
  T chanceInt(T limit) {
    return static_cast<T>(limit * chance());
  }

  float chance() {
    return dis(gen);
  }

  void generateWaterAndLand();
  void formatWaterTiles();
  void placeVegetation();
  void placeTreeBorder();
  void generateOverworld();

  bool makeCamp();

  bool generateSettlementPiece(TileRect Area, int iteration = 0);

  void generateHouse();

  void makeMineResources();
  void makeMineFloorClutter();
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
        if (!level->getBuilding2(x, y).empty())
          return false;
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
  explicit LevelGen(unsigned seed);

  Level *generate(World &world, GameData &data, Level::Type type, const Connection *C);

  const std::vector<Connection> &getOpenConnections() const {
    return openConnections;
  }

  const Connection &getBackConnection() const {
    return BackConnection;
  }
};

#endif // ARGOS_LEVELGEN_H
