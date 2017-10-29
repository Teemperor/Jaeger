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
  virtual void update(float dtime) override {
    if (isControlled()) {
      setPos(getPos().mod(getXInput() * walkSpeed * dtime, getYInput() * walkSpeed * dtime));
    }
  }

private:
  static constexpr float walkSpeed = 45;
  BodyType BodyType_ = BodyType::Pale;
  sf::Sprite sprite_, talkingSprite_, bubbleSprite_;
  bool talking_ = true;

};

#endif // CHARACTER_H
