#ifndef GAMEUI_H
#define GAMEUI_H

#include "InventoryWindow.h"
#include "PlayerControls.h"
#include "StatusBar.h"

#include <cassert>

#include <GameObject.h>
#include <gamedata/GameData.h>

class GameUI {
  sf::Sprite combatSelection, itemSelection, HealthBarStart, HealthBarMid,
      HealthBarEnd, StaminaBarStart, StaminaBarMid, StaminaBarEnd,
      StatusBackground;
  PlayerControls controls;

  bool InventoryOpen = false;

  float AbsoluteTime = 0;

  void drawInventory(sf::RenderTarget &target, float time);

  InventoryWindow *FocusedWindow = nullptr;
  InventoryWindow *MyWindow = nullptr;
  InventoryWindow *OtherWindow = nullptr;

  StatusBar HealthBar, StaminaBar;

  InventoryWindow *getUnfocusedWindow() {
    if (FocusedWindow == MyWindow)
      return OtherWindow;
    return MyWindow;
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

  void draw(sf::RenderTarget &target, float dtime);
};

#endif // GAMEUI_H
