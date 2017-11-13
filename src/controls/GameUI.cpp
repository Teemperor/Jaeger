#include "GameUI.h"

GameUI::GameUI(GameData &Data, unsigned PlayerNumber) : controls(PlayerNumber) {
  combatSelection = Data.getSprite("combat_selection");
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
}

void GameUI::draw(sf::RenderTarget &target, float time) {
  controls.update();
  if (GameObject *t = controls.getTarget()) {

    float offset = (std::abs(std::sin(time * 10.0f)) * 4.0f);
    combatSelection.setPosition(t->getPos().getX(), t->getPos().getY() - 16 - offset);
    target.draw(combatSelection);
  }
}
