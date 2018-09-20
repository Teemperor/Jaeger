#ifndef MAMBO_WALKTASK_H
#define MAMBO_WALKTASK_H

#include "AITask.h"
#include "Character.h"

class WalkTask : public AITask {
  std::vector<TilePos> WalkPath;
  TilePos Target;
  Level *LastLevel = nullptr;

  bool inTargetLevel(Character &C) {
    return &C.getLevel() == &Target.getLevel();
  }

public:
  explicit WalkTask(TilePos Target) : Target(Target) {}

  AITask *act(Character &C, float DTime) override;
};

#endif // MAMBO_WALKTASK_H
