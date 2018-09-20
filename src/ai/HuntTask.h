#ifndef MAMBO_HUNTTASK_H
#define MAMBO_HUNTTASK_H

#include "AITask.h"

class Creature;

class HuntTask : public AITask {
  Creature *Target;
  float WeaponRange = 0;
  bool InCombat = false;

public:
  explicit HuntTask(Creature *Target) : Target(Target) {}
  AITask *act(Character &C, float DTime) override;

  bool actInactive(Character &C) override;
};

#endif // MAMBO_HUNTTASK_H
