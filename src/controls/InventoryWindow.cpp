#include "InventoryWindow.h"
#include <Item.h>

InventoryWindow::InventoryWindow(GameData &Data) {

  InventoryBackground = Data.getSprite("inventory_background");
  InventoryBackground.setScale(InventoryScale, InventoryScale);

  EquipGlow = Data.getSprite("glow_icon");
  EquipGlow.setOrigin(EquipGlow.getLocalBounds().width / 2,
                      EquipGlow.getLocalBounds().height / 2);

  SelectGlow = Data.getSprite("select_icon");
  SelectGlow.setOrigin(SelectGlow.getLocalBounds().width / 2,
                       SelectGlow.getLocalBounds().height / 2);
}

void InventoryWindow::draw(sf::RenderTarget &target, bool Focused) {
  InventoryBackground.setPosition(OffsetX, OffsetY);
  target.draw(InventoryBackground);

  if (!Inv)
    return;

  int x = 0;
  int y = 0;
  for (Item &I : *Inv) {
    const float Scale = InventoryScale;
    float CenterX = x * 18 + 5;
    float CenterY = y * 18 + 5;

    if (!I.empty()) {
      sf::Sprite s = I.icon();
      s.setOrigin(s.getLocalBounds().width / 2, s.getLocalBounds().height / 2);
      s.scale(Scale, Scale);
      s.setPosition(OffsetX + (CenterX + s.getOrigin().x) * Scale,
                    OffsetY + (CenterY + s.getOrigin().y) * Scale);
      target.draw(s);
    }

    if (C && C->equipped(I)) {
      float GlowScale = Scale + 0.2f;
      EquipGlow.setScale(GlowScale, GlowScale);
      EquipGlow.setPosition(
          OffsetX + (CenterX + EquipGlow.getOrigin().x) * Scale,
          OffsetY + (CenterY + EquipGlow.getOrigin().y) * Scale);
      target.draw(EquipGlow);
    }

    if (x == InvX && y == InvY && Focused) {
      float GlowScale = Scale + 0.2f;
      SelectGlow.setScale(GlowScale, GlowScale);
      SelectGlow.setPosition(
          OffsetX + (CenterX + SelectGlow.getOrigin().x) * Scale,
          OffsetY + (CenterY + SelectGlow.getOrigin().y) * Scale);
      target.draw(SelectGlow);
    }

    ++x;
    if (x >= InventoryWidth) {
      x = 0;
      ++y;
    }
  }
}
