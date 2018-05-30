#ifndef TILEEXTRAINFO_H
#define TILEEXTRAINFO_H

#include <TilePos.h>
#include <Inventory.h>

class TileExtraInfo {
  bool teleports = false;
  TilePos teleportTarget;
  Inventory Inv;
  bool HasInventory = false;

public:
  TileExtraInfo();

  void createInventory() {
    HasInventory = true;
  }

  bool hasInventory() const {
    return HasInventory;
  }

  Inventory &getInventory() {
    assert(hasInventory());
    return Inv;
  }

  bool isTeleporting() const { return teleports; }

  void setTeleportTarget(const TilePos &pos) {
    teleportTarget = pos;
    teleports = true;
  }

  const TilePos &getTeleportTarget() const {
    assert(teleports);
    return teleportTarget;
  }
};

#endif // TILEEXTRAINFO_H
