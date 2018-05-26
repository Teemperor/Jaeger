#include "Item.h"

#include "level/Level.h"

#include <level/World.h>

Item::Item(ItemData &Data, World& W) : Data(&Data) {
  ID = W.getFreshItemID();
}

bool Item::available(Level &level) { return level.getTime() > NextAvailableTime; }

bool Item::tryUse(Level &level) {
  if (available(level)) {
    NextAvailableTime = level.getTime() + getCooldown();
    return true;
  }
  return false;
}
