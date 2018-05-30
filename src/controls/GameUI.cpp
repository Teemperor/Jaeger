#include "GameUI.h"

#include <Character.h>

GameUI::GameUI(GameData &Data, unsigned PlayerNumber) : controls(PlayerNumber) {
  combatSelection = Data.getSprite("combat_selection");
  combatSelection.scale(0.4f, 0.4f);
  itemSelection = Data.getSprite("inventory_selection");
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

  HealthBar = StatusBar(Data, "healthbar", 100);
  StaminaBar = StatusBar(Data, "staminabar", 100);

  StatusBackground = Data.getSprite("status_background");

  int InvSize = 350;



  MyWindow = new InventoryWindow(Data);
  MyWindow->setOffset(InvSize, InvSize * (PlayerNumber - 1));
  FocusedWindow = MyWindow;
  OtherWindow = new InventoryWindow(Data);
  OtherWindow->setOffset(InvSize * 2, InvSize * (PlayerNumber - 1));
}

void GameUI::draw(sf::RenderTarget &target, float dtime) {
  update();
  AbsoluteTime += dtime;

  InventoryLocation Inv = controls.getInventoryTarget();
  if (Inv.valid()) {
    float offset = (std::abs(std::sin(AbsoluteTime * 10.0f)) * 4.0f);
    itemSelection.setPosition(Inv.InvPos.getX(),
                              Inv.InvPos.getY() - 16 - offset);
    target.draw(itemSelection);
  }

  if (GameObject *t = controls.getTarget()) {
    float offset = (std::abs(std::sin(AbsoluteTime * 10.0f)) * 4.0f);
    combatSelection.setPosition(t->getPos().getX(),
                                t->getPos().getY() - 16 - offset);
    target.draw(combatSelection);
  }

  sf::View ViewBak = target.getView();
  target.setView(target.getDefaultView());

  if (Character *C = controls.getControlledCharacter()) {
    target.draw(StatusBackground);
    HealthBar.setValue(C->getHealth());
    HealthBar.setMax(C->getMaxHealth());
    HealthBar.draw(target, dtime);

    StaminaBar.setOffset(0, 20);
    StaminaBar.setValue(C->getFatigue());
    StaminaBar.setMax(C->getMaxFatigue());
    StaminaBar.draw(target, dtime);
  }

  if (InventoryOpen)
    drawInventory(target, dtime);

  target.setView(ViewBak);
}

void GameUI::drawInventory(sf::RenderTarget &target, float time) {
  MyWindow->setCharacter(controls.getControlledCharacter());
  if (MyWindow)
    MyWindow->draw(target, FocusedWindow == MyWindow);
  if (OtherWindow) {
    auto TargetInv = controls.getInventoryTarget();
    if (TargetInv.valid()) {
      OtherWindow->setInventory(TargetInv.Inv);
      OtherWindow->draw(target, FocusedWindow == OtherWindow);
    } else {
      FocusedWindow = MyWindow;
    }
  }
}

void GameUI::handleEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {
    const auto KeyCode = event.key.code;
    if (KeyCode == sf::Keyboard::Tab) {
      toggleInventory();
    }
    if (InventoryOpen && FocusedWindow) {
      bool HasOtherInv = getControls().getInventoryTarget().valid();
      switch(KeyCode) {
        case sf::Keyboard::A:
          if (HasOtherInv && FocusedWindow->isOnLeftBorder()) {
            if (FocusedWindow == MyWindow) {
              FocusedWindow = OtherWindow;
              FocusedWindow->moveToRight();
            } else if (FocusedWindow == OtherWindow){
              FocusedWindow = MyWindow;
              FocusedWindow->moveToRight();
            }
          } else {
            FocusedWindow->moveControl(-1, 0);
          }
          break;
        case sf::Keyboard::D:
          if (HasOtherInv && FocusedWindow->isOnRightBorder()) {
            if (FocusedWindow == MyWindow) {
              FocusedWindow = OtherWindow;
              FocusedWindow->moveToLeft();
            } else if (FocusedWindow == OtherWindow){
              FocusedWindow = MyWindow;
              FocusedWindow->moveToLeft();
            }
          } else {
            FocusedWindow->moveControl(1, 0);
          }
          break;
        case sf::Keyboard::W:
          FocusedWindow->moveControl(0, -1);
          break;
        case sf::Keyboard::S:
          FocusedWindow->moveControl(0, 1);
          break;
        default:
          break;
      }

      if (KeyCode == sf::Keyboard::Space) {
        if (HasOtherInv) {
          // Inventory exchange mode
          auto I = FocusedWindow->getSelectedItem();
          if (!I.empty() && getUnfocusedWindow()->getInventory()->add(I)) {
            bool RemoveResult = FocusedWindow->getInventory()->remove(I);
            assert(RemoveResult);
            controls.getControlledCharacter()->updateEquipped();
          }
        } else {
          // Equip mode
          auto C = controls.getControlledCharacter();
          if (C) {
            auto &I = FocusedWindow->getSelectedItem();
            if (!I.empty()) {
              if (I.kind() == ItemData::Consumable) {
                C->useItem(I);
              }
              else {
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
  }
}
