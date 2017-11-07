#ifndef TILEEXTRAINFO_H
#define TILEEXTRAINFO_H

#include <TilePos.h>

class TileExtraInfo {
  bool teleports = false;
  TilePos teleportTarget;

public:
  TileExtraInfo();

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
