#ifndef CHARACTER_H
#define CHARACTER_H

#include "Controlable.h"
#include "Creature.h"
#include "Inventory.h"
#include "Item.h"
#include "TilePos.h"

#include <GameObject.h>

#include <gamedata/GameData.h>

class Character : public Controlable, public Creature {

public:
  enum class BodyType { Pale, Normal, Tanned, Green };

  Character(Level &level, Vec2 pos, BodyType type = BodyType::Normal);

  void setBodyType(BodyType t);

  void render(sf::RenderTarget &target) override;
  void update(float dtime) override;

  void equipItem(const Item &i);

  Inventory &getInventory() { return Inv; }

  bool equipped(const Item &I) const {
    if (I.empty())
      return false;
    return Equipped[I.kind()] == I;
  }

  void setPlayerControls(PlayerControls *c) override {
    Controlable::setPlayerControls(c);
    c->setControlledCharacter(this);
  }

  void addItem(const Item &I) {
    if (Inv.add(I)) {
      if (Equipped[I.kind()].empty()) {
        equipItem(I);
      }
    }
  }

  void updateEquipped() {
    for (Item &I : Equipped) {
      if (!Inv.contains(I)) {
        I = Item();
      }
    }
  }

private:
  std::vector<Item> Equipped;

  std::vector<TilePos> WalkPath;

  void AIAttack(Creature &C, GameObject &o, float dtime);

  void walkToward(Vec2 pos, float dtime, bool backwards = false);

  void damage(int dmg) override;

  int getArmorValue() const {
    int Result = 0;
    for (auto &I : Equipped) {
      if (!I.empty())
        Result += I.armor();
    }
    return Result;
  }

  bool tryShootAt(GameObject &o);

  void setWalking(bool v);
  static constexpr float WalkSpeed = 45;
  BodyType TheBodyType = BodyType::Pale;
  sf::Sprite Sprite, TalkingSprite, BubbleSprite, Shadow, Gravestone, Selection;
  bool Talking = false;
  bool Walking = false;
  bool OldWalkingValue = false;
  double WalkingStartTime = 0;
  Inventory Inv;
};

#endif // CHARACTER_H
