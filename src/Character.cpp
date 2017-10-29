#include "Character.h"

#include "level/Level.h"

Character::Character(Level &level, BodyType type) : GameObject(level)
{
  setBodyType(type);
  bubbleSprite_ = getGameData().getSprite("speech_bubble");
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
    if (getLevel().getTimeModulo(0.4) > 0.2) {
      shouldUseTalkingSprite = true;
    }
    if (getLevel().getTimeModulo(3) > 0.6) {
      shouldHaveSpeechBubble = true;
    }
  }
  sf::Sprite& f = shouldUseTalkingSprite ? talkingSprite_ : sprite_;
  f.setPosition(this->getPos().getX() - 8, this->getPos().getY() - 16);
  target.draw(f);

  if (shouldHaveSpeechBubble) {
    bubbleSprite_.setPosition(this->getPos().getX() + 2, this->getPos().getY() - 22);
    target.draw(bubbleSprite_);
  }
}
