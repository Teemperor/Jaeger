#ifndef ARGOS_LEVELGEN_H
#define ARGOS_LEVELGEN_H

#include "Level.h"

#include <random>

class World;

class LevelGen {
  GameData *data;
  Level *level;
  
  void make_tree(int x, int y, bool force = false);
  void make_bush(int x, int y, float random);

  void make_house(int x, int y, int w, int h, int depth);

  void build(int x, int y, std::string tileName);
  void overlay(int x, int y, std::string tileName);

  float getHeight(int x, int y);
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<float> dis;

  void generate_overworld();

  void generate_house();
public:
  struct Connection {
    TilePos pos;
    Level::Type targetType;
  };
private:
  std::vector<Connection> openConnections;

public:
  LevelGen() : gen(rd()), dis(0, 1) {}

  Level *generate(World &world, GameData &data, Level::Type type, int w, int h);
};

#endif // ARGOS_LEVELGEN_H
