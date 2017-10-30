#ifndef ARGOS_LEVELGEN_H
#define ARGOS_LEVELGEN_H


#include "Level.h"

class LevelGen {
  GameData *data;
  Level* level;

  void make_tree(int x, int y);
  void make_bush(int x, int y, float random);

public:
  LevelGen() {
  }

  Level* generate(GameData& data, int w, int h);

};


#endif //ARGOS_LEVELGEN_H
