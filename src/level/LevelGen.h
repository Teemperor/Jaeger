#ifndef ARGOS_LEVELGEN_H
#define ARGOS_LEVELGEN_H


#include "Level.h"

class LevelGen {
  GameData *data;
  Level* level;

  void make_tree(int x, int y);
  void make_bush(int x, int y, float random);

  void make_house(int x, int y, int w, int h, int depth);

  void build(int x, int y, std::string tileName);
  void overlay(int x, int y, std::string tileName);

public:
  LevelGen() {
  }

  Level* generate(GameData& data, int w, int h);

};


#endif //ARGOS_LEVELGEN_H
