#include "GameUI.h"

#include <Character.h>

GameUI::GameUI(GameData &Data, unsigned PlayerNumber) : controls(PlayerNumber) {
  combatSelection = Data.getSprite("combat_selection");
  combatSelection.scale(0.4f, 0.4f);
  itemSelection = Data.getSprite("item_selection");
  itemSelection.scale(0.4f, 0.4f);

  switch (PlayerNumber) {
  case 1:
    combatSelection.setColor(sf::Color::Red);
    itemSelection.setColor(sf::Color::Red);
    break;
  case 2:
    combatSelection.setColor(sf::Color::Blue);
    itemSelection.setColor(sf::Color::Blue);
    break;
  default:
    assert(false && "Player number not supported");
  }

  HealthBarStart = Data.getSprite("healthbar_start");
  HealthBarMid = Data.getSprite("healthbar_mid");
  HealthBarEnd = Data.getSprite("healthbar_end");

  StaminaBarStart = Data.getSprite("staminabar_start");
  StaminaBarMid = Data.getSprite("staminabar_mid");
  StaminaBarEnd = Data.getSprite("staminabar_end");

  StatusBackground = Data.getSprite("status_background");

  InventoryBackground = Data.getSprite("inventory_background");
  InventoryBackground.setScale(InventoryScale, InventoryScale);

  EquipGlow = Data.getSprite("glow_icon");
  EquipGlow.setOrigin(EquipGlow.getLocalBounds().width / 2,
                      EquipGlow.getLocalBounds().height / 2);

  SelectGlow = Data.getSprite("select_icon");
  SelectGlow.setOrigin(SelectGlow.getLocalBounds().width / 2,
                       SelectGlow.getLocalBounds().height / 2);
}

void GameUI::draw(sf::RenderTarget &target, float time) {
  update();

  if (GameObject *t = controls.getInventoryTarget()) {
    float offset = (std::abs(std::sin(time * 10.0f)) * 4.0f);
    itemSelection.setPosition(t->getPos().getX(),
                                t->getPos().getY() - 16 - offset);
    target.draw(itemSelection);
  }

  if (GameObject *t = controls.getTarget()) {
    float offset = (std::abs(std::sin(time * 10.0f)) * 4.0f);
    combatSelection.setPosition(t->getPos().getX(),
                                t->getPos().getY() - 16 - offset);
    target.draw(combatSelection);
  }

  sf::View ViewBak = target.getView();
  target.setView(target.getDefaultView());

  if (Character *C = controls.getControlledCharacter()) {
    int MaxBarLenght = 100;
    int Barlength = (int)(MaxBarLenght * C->percentageHealth());

    target.draw(StatusBackground);
    target.draw(HealthBarStart);
    for (int i = 0; i < Barlength; ++i) {
      HealthBarMid.setPosition({(float)i, 0});
      target.draw(HealthBarMid);
    }
  }

  if (InventoryOpen)
    drawInventory(target, time);

  target.setView(ViewBak);
}

void GameUI::drawInventory(sf::RenderTarget &target, float time) {
  int InvOffsetX = 100;
  int InvOffsetY = 100;
  InventoryBackground.setPosition(InvOffsetX, InvOffsetY);
  target.draw(InventoryBackground);

  if (Character *C = controls.getControlledCharacter()) {
    int x = 0;
    int y = 0;
    for (Item &I : C->getPrivateInventory()) {
      const float scale = InventoryScale;
      float CenterX = x * 18 + 5;
      float CenterY = y * 18 + 5;

      if (!I.empty()) {
        sf::Sprite s = I.icon();
        s.setOrigin(s.getLocalBounds().width / 2,
                    s.getLocalBounds().height / 2);
        s.scale(scale, scale);
        s.setPosition(InvOffsetX + (CenterX + s.getOrigin().x) * scale,
                      InvOffsetY + (CenterY + s.getOrigin().y) * scale);
        target.draw(s);
      }

      if (C->equipped(I)) {
        float GlowScale = scale + 0.2f;
        EquipGlow.setScale(GlowScale, GlowScale);
        EquipGlow.setPosition(
            InvOffsetX + (CenterX + EquipGlow.getOrigin().x) * scale,
            InvOffsetY + (CenterY + EquipGlow.getOrigin().y) * scale);
        target.draw(EquipGlow);
      }

      if (x == InvX && y == InvY) {
        float GlowScale = scale + 0.2f;
        SelectGlow.setScale(GlowScale, GlowScale);
        SelectGlow.setPosition(
            InvOffsetX + (CenterX + SelectGlow.getOrigin().x) * scale,
            InvOffsetY + (CenterY + SelectGlow.getOrigin().y) * scale);
        target.draw(SelectGlow);
      }

      ++x;
      if (x >= InventoryWidth) {
        x = 0;
        ++y;
      }
    }
  }
}

void GameUI::handleEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {
    const auto KeyCode = event.key.code;
    if (KeyCode == sf::Keyboard::Tab) {
      toggleInventory();
    }
    if (InventoryOpen) {
      switch(KeyCode) {
        case sf::Keyboard::A:
          InvX--;
          break;
        case sf::Keyboard::D:
          InvX++;
          break;
        case sf::Keyboard::W:
          InvY--;
          break;
        case sf::Keyboard::S:
          InvY++;
          break;
        default:
          break;
      }

      // Handle wrapping.
      while(InvX < 0)
        InvX += InventoryWidth;
      while(InvY < 0)
        InvY += InventoryWidth;
      InvX %= InventoryWidth;
      InvY %= InventoryWidth;

      if (KeyCode == sf::Keyboard::Space) {
        auto C = controls.getControlledCharacter();
        if (C) {
          auto &I = C->getPrivateInventory().at(selectedItem());
          if (I.kind() == ItemData::Consumable) {
            C->useItem(I);
          } else {
            if (C->equipped(I)) {
              C->unequipItem(I);
            } else {
              C->equipItem(I);
            }
          }
        }
      }
    }
  }
}
