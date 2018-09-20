#ifndef MAMBO_DEFENSETASK_H
#define MAMBO_DEFENSETASK_H


#include <TilePos.h>
#include "AITask.h"

class DefenseTask : public AITask {
  bool First = true;
  TilePos Start;
public:
  AITask *act(Character &C, float DTime) override;
};



#endif //MAMBO_DEFENSETASK_H
