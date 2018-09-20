#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"

class Inventory {
  const static unsigned Size = 16;
  std::array<Item, Size> Slots;
  bool CanStoreItems = true;
public:
  typedef Item::Coins Coins;
private:
  Coins Gold;
public:
  void addGold(Coins C) {
    Gold += C;
  }

  bool removeGold(Coins C) {
    if (C > Gold)
      return false;
    Gold -= C;
    return true;
  }

  bool canBuyItem(const Inventory &Other, const Item &I) const {
    if (full())
      return false;
    if (!Other.contains(I))
      return false;
    return Gold >= I.price();
  }

  bool buyItem(Inventory &Other, Item &I) {
    if (!canBuyItem(Other, I))
      return false;
    Item Copy = I;
    Other.remove(I);
    this->add(Copy);

    this->removeGold(I.price());
    Other.addGold(I.price());

    return true;
  }

  bool add(const Item &I) {
    for (auto &Slot : Slots) {
      if (Slot.empty()) {
        Slot = I;
        return true;
      }
    }
    return false;
  }

  void disableItemStorage() { CanStoreItems = false; }

  unsigned emptySpaces() const;

  void takeAll(Inventory &Other);

  bool empty() const {
    for (auto &Slot : Slots) {
      if (!Slot.empty()) {
        return false;
      }
    }
    return true;
  }

  bool full() const {
    for (auto &Slot : Slots) {
      if (Slot.empty()) {
        return false;
      }
    }
    return true;
  }

  bool canStoreItems() const { return CanStoreItems; }

  bool remove(const Item &I) {
    for (auto &Slot : Slots) {
      if (Slot == I) {
        Slot = Item();
        return true;
      }
    }
    return false;
  }

  Item &at(std::size_t I) { return Slots.at(I); }

  const Item &at(std::size_t I) const { return Slots.at(I); }

  bool contains(const Item &I) const {
    return std::find(Slots.begin(), Slots.end(), I) != Slots.end();
  }

  typedef std::array<Item, Size>::iterator Iter;

  Iter begin() { return Slots.begin(); }

  Iter end() { return Slots.end(); }
};

#endif // INVENTORY_H
