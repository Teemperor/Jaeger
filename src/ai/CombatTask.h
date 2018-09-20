#ifndef MAMBO_COMBATTASK_H
#define MAMBO_COMBATTASK_H

#include "AITask.h"
#include <Creature.h>

class CombatTask : public AITask {
  Creature *Target;

public:
  explicit CombatTask(Creature *Target) : Target(Target) {}

  AITask *act(Character &C, float DTime) override;
};

#endif // MAMBO_COMBATTASK_H
