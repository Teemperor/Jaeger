#ifndef MAMBO_INVENTORYWINDOW_H
#define MAMBO_INVENTORYWINDOW_H

#include <Character.h>
#include <SFML/Graphics.hpp>
#include <gamedata/GameData.h>

class InventoryWindow {
  sf::Sprite InventoryBackground, EquipGlow, SelectGlow;
  float InventoryScale = 4;
  Character *C = nullptr;
  Inventory *Inv = nullptr;

  int InvX = 1;
  int InvY = 1;

  int InventoryWidth = 4;

  int OffsetX = 0;
  int OffsetY = 0;

public:
  explicit InventoryWindow(GameData &Data);
  void draw(sf::RenderTarget &target, bool Focused);

  void setCharacter(Character *NewC) {
    C = NewC;
    Inv = &C->getPrivateInventory();
  }

  void setOffset(int x, int y) {
    OffsetX = x;
    OffsetY = y;
  }

  void setInventory(Inventory *I) { Inv = I; }

  Inventory *getInventory() { return Inv; }

  Item &getSelectedItem() {
    assert(Inv);
    return Inv->at(selectedItem());
  }

  bool isOnLeftBorder() const { return InvX == 0; }

  bool isOnRightBorder() const { return InvX == 3; }

  void moveToLeft() { InvX = 0; }

  void moveToRight() { InvX = 3; }

  void moveControl(int x, int y) {
    InvX += x;
    InvY += y;

    // Handle wrapping.
    while (InvX < 0)
      InvX += InventoryWidth;
    while (InvY < 0)
      InvY += InventoryWidth;
    InvX %= InventoryWidth;
    InvY %= InventoryWidth;
  }

  std::size_t selectedItem() {
    return static_cast<size_t>(InvX + InvY * InventoryWidth);
  }
};

#endif // MAMBO_INVENTORYWINDOW_H
