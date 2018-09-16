#include <Inventory.h>
#include <iostream>

void Inventory::takeAll(Inventory &Other) {
  for (auto &I : Other) {
    if (I.empty())
      continue;
    if (full())
      break;
    Item ToAdd = I;
    if (Other.remove(I)) {
      bool added = add(ToAdd);
      assert(added);
    }
  }
}

unsigned Inventory::emptySpaces() const {
  unsigned Result = 0;
  for (auto &Slot : Slots) {
    if (Slot.empty())
      ++Result;
  }
  return Result;
}
