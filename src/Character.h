#ifndef CHARACTER_H
#define CHARACTER_H

#include "Controlable.h"
#include "Creature.h"
#include "Inventory.h"
#include "Item.h"
#include "TilePos.h"
#include <ai/AITask.h>

#include <GameObject.h>

#include <gamedata/GameData.h>

class Character : public Controlable, public Creature {
public:
  friend class AITask;

  enum class BodyType { Pale, Normal, Tanned, Green };

  Character(Level &level, Vec2 pos, BodyType type = BodyType::Normal);

  void setBodyType(BodyType t);

  void render(sf::RenderTarget &target) override;
  void update(float dtime) override;

  void equipItem(const Item &I);
  void unequipItem(const Item &I);

  Inventory &getPrivateInventory() { return Inv; }

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
      if (I.kind() != ItemData::Consumable &&
          Equipped[I.kind()].empty()) {
        equipItem(I);
      }
    }
  }

  void removeItem(const Item &I) {
    if (Inv.remove(I)) {
      updateEquipped();
    }
  }

  void updateEquipped() {
    for (Item &I : Equipped) {
      if (!Inv.contains(I)) {
        I = Item();
      }
    }
  }

  std::vector<Creature *> getClosestEnemies(float distance);

  const Item &getEquipped(ItemData::Kind Kind) {
    return Equipped.at(Kind);
  }

  void useItem(Item &item);

  void walkToward(Vec2 pos, float dtime, bool backwards = false);
  bool tryShootAt(GameObject &o);
private:
  std::vector<Item> Equipped;

  void damage(int dmg) override;

  int getArmorValue() const {
    int Result = 0;
    for (auto &I : Equipped) {
      if (!I.empty())
        Result += I.armor();
    }
    return Result;
  }

  void onDeath() {
    setInventory(&Inv);
  }

  void setWalking(bool v);
  static constexpr float WalkSpeed = 45;
  BodyType TheBodyType = BodyType::Pale;
  sf::Sprite Sprite, TalkingSprite, BubbleSprite, Shadow, Gravestone, Selection;
  bool Talking = false;
  bool Walking = false;
  bool OldWalkingValue = false;
  double WalkingStartTime = 0;
  Inventory Inv;
  CharacterAI MyAI;
};

#endif // CHARACTER_H
