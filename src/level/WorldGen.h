#ifndef WORLDGEN_H
#define WORLDGEN_H

#include "World.h"

class WorldGen {
public:
  WorldGen();

  World* generate(GameData &d);
};

#endif // WORLDGEN_H
