#ifndef GAMEUI_H
#define GAMEUI_H

#include "PlayerControls.h"

#include <cassert>

#include <GameObject.h>
#include <gamedata/GameData.h>

class GameUI {
  sf::Sprite combatSelection, HealthBarStart, HealthBarMid, HealthBarEnd,
      StaminaBarStart, StaminaBarMid, StaminaBarEnd, StatusBackground,
      InventoryBackground, EquipGlow, SelectGlow;
  PlayerControls controls;

  float InventoryScale = 4;

  int InventoryWidth = 4;

  int InvX = 1;
  int InvY = 1;

  bool InventoryOpen = false;

  void drawInventory(sf::RenderTarget &target, float time);

  std::size_t selectedItem() {
    return static_cast<size_t>(InvX + InvY * InventoryWidth);
  }

public:
  GameUI(GameData &Data, unsigned PlayerNumber);

  PlayerControls &getControls() { return controls; }

  void handleEvent(sf::Event event);
  void update() {
    if (!InventoryOpen) {
      controls.update();
    }
  }

  void toggleInventory() { InventoryOpen = !InventoryOpen; }

  void draw(sf::RenderTarget &target, float time);
};

#endif // GAMEUI_H
