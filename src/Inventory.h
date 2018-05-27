#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"

class Inventory {
  const static unsigned Size = 16;
  std::array<Item, Size> Slots;

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

  bool remove(const Item &I) {
    for (auto &Slot : Slots) {
      if (Slot == I) {
        Slot = Item();
        return true;
      }
    }
    return false;
  }

  Item& at(std::size_t I) {
    return Slots.at(I);
  }

  const Item& at(std::size_t I) const {
    return Slots.at(I);
  }

  bool contains(Item &I) const {
    return std::find(Slots.begin(), Slots.end(), I) != Slots.end();
  }

  typedef std::array<Item, Size>::iterator Iter;

  Iter begin() { return Slots.begin(); }

  Iter end() { return Slots.end(); }
};

#endif // INVENTORY_H
