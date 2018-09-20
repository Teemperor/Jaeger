#include "DefenseTask.h"
#include "HuntTask.h"
#include "WalkTask.h"

#include <Character.h>

AITask *DefenseTask::act(Character &C, float DTime) {
  if (First) {
    Start = C.getTilePos();
    First = false;
  }

  auto ClosestEnemies = C.getClosestEnemies(16 * 20);
  if (!ClosestEnemies.empty()) {
    return new HuntTask(ClosestEnemies.front());
  }
  return new WalkTask(Start);
}
