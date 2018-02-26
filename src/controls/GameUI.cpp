#include "GameUI.h"

#include <Character.h>

GameUI::GameUI(GameData &Data, unsigned PlayerNumber) : controls(PlayerNumber) {
  combatSelection = Data.getSprite("combat_selection");
  combatSelection.scale(0.4f, 0.4f);
  switch (PlayerNumber) {
  case 1:
    combatSelection.setColor(sf::Color::Red);
    break;
  case 2:
    combatSelection.setColor(sf::Color::Blue);
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
}

void GameUI::draw(sf::RenderTarget &target, float time) {
  controls.update();
  if (GameObject *t = controls.getTarget()) {
    float offset = (std::abs(std::sin(time * 10.0f)) * 4.0f);
    combatSelection.setPosition(t->getPos().getX(), t->getPos().getY() - 16 - offset);
    target.draw(combatSelection);
  }

  sf::View ViewBak = target.getView();
  target.setView(target.getDefaultView());


  if (Character *C = controls.getControlledCharacter()) {
    int MaxBarLenght = 100;
    int Barlength = (int) (MaxBarLenght * C->percentageHealth());

    float p = C->percentageHealth();
    target.draw(StatusBackground);
    target.draw(HealthBarStart);
    for (int i = 0; i < Barlength; ++i) {
      HealthBarMid.setPosition({i, 0});
      target.draw(HealthBarMid);
    }
  }

  target.setView(ViewBak);
}
