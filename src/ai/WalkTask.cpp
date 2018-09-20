#include "WalkTask.h"

#include <PathFinder.h>

AITask *WalkTask::act(Character &C, float DTime) {
  if (&C.getLevel() != LastLevel) {
    LastLevel = &C.getLevel();
    WalkPath.clear();
  }

  if (WalkPath.empty()) {
    PathFinder finder(C.getLevel());
    finder.findPath(C.getTilePos(), Target, WalkPath);
  }

  if (WalkPath.empty())
    return finish("Couldn't find a path");

  if (Vec2(WalkPath.back()).distance(C.getPos()) < 3.0f)
    WalkPath.pop_back();

  if (WalkPath.empty()) {
    if (!inTargetLevel(C))
      return finish("Finished path");
  } else
    C.walkToward(WalkPath.back(), DTime);

  return nullptr;
}