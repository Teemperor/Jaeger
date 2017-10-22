#ifndef ARGOS_LEVELGEN_H
#define ARGOS_LEVELGEN_H


#include "Level.h"

class LevelGen {
  GameData *data;
  Level* level;

public:
  LevelGen() {
  }

  Level* generate(GameData& data, int w, int h);

};


#endif //ARGOS_LEVELGEN_H
