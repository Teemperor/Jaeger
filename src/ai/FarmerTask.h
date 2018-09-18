#ifndef MAMBO_FARMERTASK_H
#define MAMBO_FARMERTASK_H


#include <TilePos.h>
#include "AITask.h"

class FarmerTask : public AITask {
  bool First = true;
  TilePos House;
  TilePos BeforeHouse;
public:
  AITask *act(Character &C, float DTime) override;
  AITask *actInside(Character &C);
  AITask *actOutside(Character &C);
};


#endif //MAMBO_FARMERTASK_H
