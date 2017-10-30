#include "Character.h"

#include "level/Level.h"

Character::Character(Level &level, Vec2 pos, BodyType type) : GameObject(level)
{
  setBodyType(type);
  bubbleSprite_ = getGameData().getSprite("speech_bubble");
  shadow_ = getGameData().getSprite("shadow");
  setPos(pos);
}

void Character::setBodyType(Character::BodyType t) {
  BodyType_ = t;
  std::string SpriteName = "";
  switch(t) {
    case Character::BodyType::Pale:
      SpriteName  = "body_pale";
      break;
    case Character::BodyType::Tanned:
      SpriteName  = "body_tanned";
      break;
    case Character::BodyType::Green:
      SpriteName  = "body_green";
      break;
    case Character::BodyType::Normal:
      SpriteName  = "body_normal";
      break;
  }
  sprite_ = getGameData().getSprite(SpriteName);
  talkingSprite_ = getGameData().getSprite(SpriteName + "_talking");
}

void Character::render(sf::RenderTarget &target) {
  bool shouldUseTalkingSprite = false;
  bool shouldHaveSpeechBubble = false;
  if (talking_) {
    if (getLevel().getTimeModulo(0.4) > 0.2 || getLevel().getTimeModulo(2.5) > 2.1) {
      shouldUseTalkingSprite = true;
    }
    if (getLevel().getTimeModulo(3) > 0.6) {
      shouldHaveSpeechBubble = true;
    }
  }
  shadow_.setPosition(this->getPos().getX() - 8, this->getPos().getY() - 2);
  target.draw(shadow_);

  int offset = 0;
  if (walking_) {
    offset = (int)(std::abs(std::sin((getLevel().getTime() - walkingStartTime_) * 10)) * 4);
  }
  {
    sf::Sprite& f = shouldUseTalkingSprite ? talkingSprite_ : sprite_;
    f.setPosition(this->getPos().getX() - 8, this->getPos().getY() - 16 - offset);
    target.draw(f);
  }
  if (shouldHaveSpeechBubble) {
    bubbleSprite_.setPosition(this->getPos().getX() + 2, this->getPos().getY() - 22);
    target.draw(bubbleSprite_);
  }

}

void Character::update(float dtime) {
  if (isControlled()) {
    float dx = getXInput() * walkSpeed * dtime;
    float dy = getYInput() * walkSpeed * dtime;
    if (dy < 0 && !getLevel().passable(getPos().modY(dy - 3)))
      dy = 0;
    else if (dy > 0 && !getLevel().passable(getPos().modY(dy + 3)))
      dy = 0;
    if (dx < 0 && !getLevel().passable(getPos().modX(dx - 4)))
      dx = 0;
    else if (dx > 0 && !getLevel().passable(getPos().modX(dx + 4)))
      dx = 0;
    setPos(getPos().mod(dx, dy));
    setWalking(std::abs(dx) + std::abs(dy) > 0.1f);
  }
  talking_ = !isControlled();
}

void Character::setWalking(bool v) {
  if (v != walking_) {
    walking_ = true;
    walkingStartTime_ = getLevel().getTime();
  }
}
