#ifndef TILEEXTRAINFO_H
#define TILEEXTRAINFO_H

#include <Inventory.h>
#include <TilePos.h>

class TileData;

class TileExtraInfo {
  bool teleports = false;
  TilePos teleportTarget;
  Inventory Inv;
  bool HasInventory = false;
  unsigned ResourceCounterLeft = 0;

public:
  TileExtraInfo();

  void updateResources(World &W, TileData &D, unsigned Seed);

  void createInventory() { HasInventory = true; }

  bool hasInventory() const { return HasInventory; }

  Inventory &getInventory() {
    assert(hasInventory());
    return Inv;
  }

  bool isTeleporting() const { return teleports; }

  void setTeleportTarget(const TilePos &pos) {
    teleportTarget = pos;
    teleports = true;
  }

  const TilePos &getTeleportTarget() const { return teleportTarget; }
};

#endif // TILEEXTRAINFO_H
