#include "FarmerWifeTask.h"
#include "SellTask.h"
#include "WaitTask.h"
#include "WalkTask.h"

#include <Character.h>
#include <level/Level.h>

static bool hasCorn(Character &C) {
  for (auto &I : C.getPrivateInventory())
    if (!I.empty() && I.getID() == "corn")
      return true;
  return false;
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

static bool sameLevel(TilePos a, TilePos b) {
  return &a.getLevel() == &b.getLevel();
}

AITask *FarmerWifeTask::act(Character &C, float DTime) {
  if (First) {
    House = C.getTilePos();
    BeforeHouse = getFrontOfHouse(C.getTilePos()).modY(1);
    First = false;
  }

  if (sameLevel(House, C.getTilePos())) {
    return actInside(C);
  } else if (sameLevel(BeforeHouse, C.getTilePos())) {
    return actOutside(C);
  }
  return nullptr;
}

AITask *FarmerWifeTask::actInside(Character &C) {
  if (!hasCorn(C)) {
    TilePos nextContainer = C.getLevel().searchClosestMatchingTile(
        C.getTilePos(),
        [&C](int x, int y) {
          Tile &T = C.getLevel().getBuilding(x, y);
          if (T.name() == "open_barrel") {
            if (T.getInventory() && !T.getInventory()->empty())
              return true;
          }
          return false;
        },
        8);
    if (nextContainer.valid()) {
      if (Vec2(nextContainer).distance(C.getTilePos()) <= 30) {
        Inventory *Inv =
            C.getLevel()
                .getBuilding(nextContainer.getX(), nextContainer.getY())
                .getInventory();
        if (Inv) {
          C.getPrivateInventory().takeAll(*Inv);
          return new WaitTask(3);
        }
      } else
        return WalkTask::Beside(nextContainer);
    } else {
      return new WaitTask(5);
    }
  }
  return new WalkTask(BeforeHouse);
}

AITask *FarmerWifeTask::actOutside(Character &C) {
  if (!hasCorn(C)) {
    return new WalkTask(House);
  } else {
    TilePos nextMarket = C.getLevel().searchClosestMatchingTile(
        C.getTilePos(),
        [&C](int x, int y) {
          Tile &T = C.getLevel().get(x, y);
          if (T.name() == "stone_c") {
            Tile &T2 = C.getLevel().getBuilding(x, y - 1);
            return T2.group() == "stall";
          }
          return false;
        },
        60);
    if (nextMarket.valid()) {
      if (Vec2(nextMarket).distance(C.getTilePos()) <= 10)
        return new SellTask();
      else
        return new WalkTask(nextMarket);
    } else
      return new WalkTask(House);
  }
  return nullptr;
}
