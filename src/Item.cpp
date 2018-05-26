#include "Item.h"

#include "level/Level.h"

#include <level/World.h>

Item::Item(ItemData &Data, World& W) : Data_(&Data) {
  Id_ = W.getFreshItemID();
}

bool Item::available(Level &level) { return level.getTime() > nextAvailable; }

bool Item::tryUse(Level &level) {
  if (available(level)) {
    nextAvailable = level.getTime() + getCooldown();
    return true;
  }
  return false;
}
