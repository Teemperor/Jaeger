#ifndef MAMBO_VILLAGEGUARDTASK_H
#define MAMBO_VILLAGEGUARDTASK_H

#include "AITask.h"
#include <TilePos.h>
#include <random>

class VillageGuardTask : public AITask {
  bool First = true;
  TilePos House;
  TilePos BeforeHouse;

  std::random_device rd;
  std::mt19937 gen;
  std::normal_distribution<> PosDis;

public:
  VillageGuardTask() : rd(), gen(rd()), PosDis(0, 20) {}
  AITask *act(Character &C, float DTime) override;
};

#endif // MAMBO_VILLAGEGUARDTASK_H
