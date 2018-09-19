#include "FarmerTask.h"
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


AITask *FarmerTask::act(Character &C, float DTime) {
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

AITask *FarmerTask::actInside(Character &C) {
  if (hasCorn(C)) {
    TilePos nextContainer = C.getLevel().searchClosestMatchingTile(C.getTilePos(),
                                                                   [&C](int x, int y) {
                                                                     Tile &T = C.getLevel().getBuilding(x, y);
                                                                     if (T.name() == "open_barrel") {
                                                                       if (T.getInventory() && !T.getInventory()->full())
                                                                         return true;
                                                                     }
                                                                     return false;
                                                                   }, 8);
    if (nextContainer.valid()) {
      if (Vec2(nextContainer).distance(C.getTilePos()) <= 30) {
        Inventory *Inv = C.getLevel().getBuilding(nextContainer.getX(), nextContainer.getY()).getInventory();
        if (Inv) {
          Inv->takeAll(C.getPrivateInventory());
          return new WaitTask(3);
        }
      } else
        return new WalkTask(nextContainer.modX(-1));
    }
  }
  return new WalkTask(BeforeHouse);
}

AITask *FarmerTask::actOutside(Character &C) {
  if (C.getPrivateInventory().full()) {
    return new WalkTask(House);
  } else {
    TilePos nextCorn = C.getLevel().searchClosestMatchingTile(C.getTilePos(),
                                                            [&C](int x, int y) {
                                                              Tile &T = C.getLevel().getBuilding2(x, y);
                                                              if (T.name() == "corn") {
                                                                if (T.getInventory() && !T.getInventory()->empty())
                                                                  return true;
                                                              }
                                                              return false;
                                                            }, 30);
    if (nextCorn.valid()) {
      if (Vec2(nextCorn).distance(C.getTilePos()) <= 10) {
        Inventory *Inv = C.getLevel().getBuilding2(nextCorn.getX(), nextCorn.getY()).getInventory();
        if (Inv) {
          C.getPrivateInventory().takeAll(*Inv);
          return new WaitTask(1);
        }
      } else
        return new WalkTask(nextCorn);
    } else
      return new WalkTask(House);
  }
  return nullptr;
}
