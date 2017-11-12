#include "Item.h"

#include "level/Level.h"

Item::Item(ItemData &Data) : Data_(&Data) {}

bool Item::available(Level &level) { return level.getTime() > nextAvailable; }

bool Item::tryUse(Level &level) {
  if (available(level)) {
    nextAvailable = level.getTime() + getCooldown();
    return true;
  }
  return false;
}
