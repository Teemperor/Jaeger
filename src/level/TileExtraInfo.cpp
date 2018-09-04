#include "TileExtraInfo.h"

#include <level/World.h>
#include <gamedata/TileData.h>

TileExtraInfo::TileExtraInfo() {}

void TileExtraInfo::updateResources(World &W, TileData &D, unsigned Seed) {
  if (ResourceCounterLeft <= 1 && Inv.empty()) {
    ResourceCounterLeft = D.getResourceCounter();
    auto NewItem = W.getGameData().itemGroup(D.getResources())->get(Seed);
    for (unsigned I = 0; I < NewItem.first; ++I) {
      Inv.add(Item(*NewItem.second, W));
    }
  } else if (ResourceCounterLeft > 0) {
    ResourceCounterLeft--;
  }
}
