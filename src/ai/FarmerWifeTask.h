#ifndef MAMBO_FARMERWIFETASK_H
#define MAMBO_FARMERWIFETASK_H


#include <TilePos.h>
#include "AITask.h"

class FarmerWifeTask : public AITask {
  bool First = true;
  TilePos House;
  TilePos BeforeHouse;
public:
  AITask *act(Character &C, float DTime) override;
  AITask *actInside(Character &C);
  AITask *actOutside(Character &C);
};



#endif //MAMBO_FARMERWIFETASK_H
