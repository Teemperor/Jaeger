#ifndef CHARACTER_H
#define CHARACTER_H

#include "Controlable.h"
#include "Creature.h"
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

private:
  std::vector<Item> equipped_;

  std::vector<TilePos> walkPath_;

  void AIAttack(Creature &C, GameObject &o, float dtime);

  void walkToward(Vec2 pos, float dtime);

  void damage(int dmg) override { reduceHealth(dmg); }

  void tryShootAt(GameObject &o);

  void setWalking(bool v);
  static constexpr float walkSpeed = 45;
  BodyType BodyType_ = BodyType::Pale;
  sf::Sprite sprite_, talkingSprite_, bubbleSprite_, shadow_, gravestone_,
      selection_;
  bool talking_ = false;
  bool walking_ = false;
  bool oldWalkingValue_ = false;
  double walkingStartTime_ = 0;
};

#endif // CHARACTER_H
