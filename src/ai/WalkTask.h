#ifndef MAMBO_WALKTASK_H
#define MAMBO_WALKTASK_H

#include "AITask.h"
#include "Character.h"

class WalkTask : public AITask {
  std::vector<TilePos> WalkPath;
  TilePos Target;
  bool Adjacent;
  Level *LastLevel = nullptr;

  bool inTargetLevel(Character &C) {
    return &C.getLevel() == &Target.getLevel();
  }

public:
  explicit WalkTask(TilePos Target, bool Adjacent = false)
      : Target(Target), Adjacent(Adjacent) {}
  static WalkTask *Beside(TilePos Target);

  AITask *act(Character &C, float DTime) override;

  void debugDraw(Character &C, sf::RenderTarget &target) override;
};

#endif // MAMBO_WALKTASK_H
