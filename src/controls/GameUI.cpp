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

void GameUI::draw(sf::RenderTarget &target) {
  controls.update();
  if (GameObject *t = controls.getTarget()) {
    combatSelection.setPosition(t->getPos().getX(), t->getPos().getY() - 16);
    target.draw(combatSelection);
  }
}
