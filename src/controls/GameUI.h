#ifndef GAMEUI_H
#define GAMEUI_H

#include "PlayerControls.h"

#include <cassert>

#include <GameObject.h>
#include <gamedata/GameData.h>

class GameUI {
  sf::Sprite combatSelection,
  HealthBarStart, HealthBarMid, HealthBarEnd,
  StaminaBarStart, StaminaBarMid, StaminaBarEnd,
  StatusBackground;
  PlayerControls controls;

public:
  GameUI(GameData &Data, unsigned PlayerNumber);

  PlayerControls &getControls() { return controls; }

  void draw(sf::RenderTarget &target, float time);
};

#endif // GAMEUI_H
