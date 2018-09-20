#include "WalkTask.h"

#include <PathFinder.h>

AITask *WalkTask::act(Character &C, float DTime) {
  if (&C.getLevel() != LastLevel) {
    LastLevel = &C.getLevel();
    WalkPath.clear();
  }

  if (WalkPath.empty()) {
    PathFinder finder(C.getLevel());
    finder.findPath(C.getTilePos(), Target, Adjacent, WalkPath);
  }

  if (WalkPath.empty())
    return finish("Couldn't find a path");

  if (!WalkPath.back().getLevel().passable(WalkPath.back())) {
    WalkPath.pop_back();
    return nullptr;
  }

  if (Vec2(WalkPath.back()).distance(C.getPos()) < 5.0f)
    WalkPath.pop_back();

  if (WalkPath.empty()) {
    if (!inTargetLevel(C))
      return finish("Finished path");
  } else
    C.walkToward(WalkPath.back(), DTime);

  return nullptr;
}

void WalkTask::debugDraw(Character &C, sf::RenderTarget &target) {
  std::vector<sf::Vertex> line;
  for (int i = 0; i < (int)WalkPath.size() - 1; i++) {
    Vec2 currentPos = WalkPath[i];
    Vec2 nextPos = WalkPath[i + 1];
    line.push_back(sf::Vertex(
        sf::Vector2f(currentPos.getX(), currentPos.getY()), sf::Color::Red));
    line.push_back(sf::Vertex(sf::Vector2f(nextPos.getX(), nextPos.getY()),
                              sf::Color::Red));
  }
  target.draw(line.data(), line.size(), sf::Lines);
}

WalkTask *WalkTask::Beside(TilePos Target) {
  return new WalkTask(Target, true);
}
