#ifndef ARGOS_LEVELGEN_H
#define ARGOS_LEVELGEN_H

#include "Level.h"

#include <Perlin2D.h>
#include <random>

class World;

class LevelGen {
  GameData *data;
  Level *level;

  void make_tree(int x, int y, bool force = false);
  void make_bush(int x, int y, float random);

  void make_house(int x, int y, int w, int h, int depth);

  void floor(int x, int y, std::string tileName);
  void build(int x, int y, std::string tileName);
  void overlay(int x, int y, std::string tileName);

  Perlin2D heightPerlin;
  Perlin2D woodPerlin;

  float getHeight(int x, int y);
  float getVegetation(int x, int y);
  std::mt19937 gen;
  std::uniform_real_distribution<float> dis;

  void generate_overworld();

  void generate_house();

  enum TileCompare {
    CMP_S, // Same group
    CMP_D, // DIfferent group
    CMP_A // all groups
  };
  bool hasSurrounding(int x, int y, const std::string &group, std::array<TileCompare, 8> surrounding);
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
