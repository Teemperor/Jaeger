#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"

class Inventory {
  std::array<Item, 16> Slots;

public:
  bool add(const Item &I) {
    for (auto &Slot : Slots) {
      if (Slot.empty()) {
        Slot = I;
        return true;
      }
    }
    return false;
  }

  bool contains(Item &I) const {
    return std::find(Slots.begin(), Slots.end(), I) != Slots.end();
  }

  std::array<Item, 16>::iterator begin() { return Slots.begin(); }

  std::array<Item, 16>::iterator end() { return Slots.end(); }
};

#endif // INVENTORY_H
