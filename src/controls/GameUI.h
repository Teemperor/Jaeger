#ifndef GAMEUI_H
#define GAMEUI_H

#include "PlayerControls.h"

#include <GameObject.h>
#include <gamedata/GameData.h>

class GameUI {
  sf::Sprite combatSelection;
  PlayerControls controls;

public:
  GameUI(GameData& Data, unsigned PlayerNumber);

  PlayerControls& getControls() {
    return controls;
  }

  void draw(sf::RenderTarget& target);
};

#endif // GAMEUI_H
