#include "VillageGuardTask.h"
#include "ListTask.h"
#include "WaitTask.h"
#include "WalkTask.h"
#include "BuyGroceriesTask.h"

#include <Character.h>
#include <level/Level.h>

static bool sameLevel(TilePos a, TilePos b) {
  return &a.getLevel() == &b.getLevel();
}

static TilePos getFrontOfHouse(TilePos p) {
  auto &L = p.getLevel();
  for (auto C : L.getConnections()) {
    if (C.getTargetLevel()->getType() == Level::Type::Overworld) {
      return C.getTargetPos();
    }
  }
  return TilePos();
}

AITask *VillageGuardTask::act(Character &C, float DTime) {
  if (First) {
    House = C.getTilePos();
    BeforeHouse = getFrontOfHouse(C.getTilePos()).modY(1);
    First = false;
  }

  if (sameLevel(House, C.getTilePos())) {
    return new WalkTask(BeforeHouse);
  } else if (sameLevel(BeforeHouse, C.getTilePos())) {
    if (Rounds > 2) {
      Rounds = 0;
      return new BuyGroceriesTask();
    }
    int dx = (int)PosDis(gen);
    int dy = (int)PosDis(gen);

    TilePos Target(BeforeHouse.getLevel(), BeforeHouse.getX() + dx,
                   BeforeHouse.getY() + dy);
    TilePos Out;
    if (C.getLevel().getTeleportTarget(Target, Out))
      return nullptr;
    Rounds++;
    return new ListTask({new WalkTask(Target), new WaitTask(5)});
  }
  return nullptr;
}