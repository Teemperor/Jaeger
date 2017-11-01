#ifndef CHARACTER_H
#define CHARACTER_H

#include "Controlable.h"
#include "Creature.h"
#include "Item.h"

#include <GameObject.h>

#include <gamedata/GameData.h>

class Character : public GameObject, public Controlable, public Creature {

public:
  enum class BodyType { Pale, Normal, Tanned, Green };

  Character(Level &level, Vec2 pos, BodyType type = BodyType::Normal);

  void setBodyType(BodyType t);

  virtual void render(sf::RenderTarget &target) override;
  virtual void update(float dtime) override;

  void equipItem(const Item &i);

private:
  std::vector<Item> equipped_;

  void AIAttack(Creature &C, GameObject &o, float dtime);

  void walkToward(Vec2 pos, float dtime);

  void setWalking(bool v);
  static constexpr float walkSpeed = 45;
  BodyType BodyType_ = BodyType::Pale;
  sf::Sprite sprite_, talkingSprite_, bubbleSprite_, shadow_, gravestone_;
  bool talking_ = false;
  bool walking_ = false;
  bool oldWalkingValue_ = false;
  double walkingStartTime_ = 0;
};

#endif // CHARACTER_H
