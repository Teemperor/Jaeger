#ifndef CHARACTER_H
#define CHARACTER_H

#include "Controlable.h"

#include <GameObject.h>

#include <gamedata/GameData.h>

class Character : public GameObject, public Controlable {

public:
  enum class BodyType {
    Pale,
    Normal,
    Tanned,
    Green
  };

  Character(Level& level, BodyType type = BodyType::Normal);

  void setBodyType(BodyType t);

  virtual void render(sf::RenderTarget& target) override;
  virtual void update(float dtime) override;

private:

  void setWalking(bool v);
  static constexpr float walkSpeed = 45;
  BodyType BodyType_ = BodyType::Pale;
  sf::Sprite sprite_, talkingSprite_, bubbleSprite_, shadow_;
  bool talking_ = true;
  bool walking_ = true;
  double walkingStartTime_ = 0;

};

#endif // CHARACTER_H
