#ifndef MAMBO_ITEMBAG_H
#define MAMBO_ITEMBAG_H

#include "GameObject.h"
#include "items/Inventory.h"

class ItemBag : public GameObject {
  Inventory I;

public:
  ItemBag(Level &l, Vec2 Pos) : GameObject(l) {
    setPos(Pos);
    setInventory(&I);
  }
};

#endif // MAMBO_ITEMBAG_H
