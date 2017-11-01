#include "Character.h"

#include "level/Level.h"

Character::Character(Level &level, Vec2 pos, BodyType type)
    : GameObject(level) {
  setBodyType(type);
  bubbleSprite_ = getGameData().getSprite("speech_bubble");
  shadow_ = getGameData().getSprite("shadow");
  gravestone_ =
      getGameData().getSprite("gravestone" + std::to_string(rand() % 9 + 1));
  setPos(pos);
  equipped_.resize(ItemData::KindLimit);

  equipItem(Item(*level.getData().item("leather_pants")));
  equipItem(Item(*level.getData().item("leather_armor")));
  equipItem(Item(*level.getData().item("steel_buckler")));
  equipItem(Item(*level.getData().item("red_steel_helmet")));
}

void Character::setBodyType(Character::BodyType t) {
  BodyType_ = t;
  std::string SpriteName = "";
  switch (t) {
  case Character::BodyType::Pale:
    SpriteName = "body_pale";
    break;
  case Character::BodyType::Tanned:
    SpriteName = "body_tanned";
    break;
  case Character::BodyType::Green:
    SpriteName = "body_green";
    break;
  case Character::BodyType::Normal:
    SpriteName = "body_normal";
    break;
  }
  sprite_ = getGameData().getSprite(SpriteName);
  talkingSprite_ = getGameData().getSprite(SpriteName + "_talking");
}

void Character::render(sf::RenderTarget &target) {
  bool shouldUseTalkingSprite = false;
  bool shouldHaveSpeechBubble = false;
  if (talking_) {
    if (getLevel().getTimeModulo(0.4) > 0.2 ||
        getLevel().getTimeModulo(2.5) > 2.1) {
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
    offset = (int)(std::abs(std::sin(
                       (getLevel().getTime() - walkingStartTime_) * 10)) *
                   4);
  }
  if (isDead()) {
    sf::Sprite &f = gravestone_;
    f.setPosition(this->getPos().getX() - 8,
                  this->getPos().getY() - 16 - offset);
    target.draw(f);
    return;
  } else {
    sf::Sprite &f = shouldUseTalkingSprite ? talkingSprite_ : sprite_;
    f.setPosition(this->getPos().getX() - 8,
                  this->getPos().getY() - 16 - offset);
    target.draw(f);
  }
  for (unsigned layer = 0; layer < equipped_.size(); layer++) {
    Item &i = equipped_.at(layer);
    if (!i.empty()) {
      sf::Sprite &f = i.sprite();
      f.setPosition(this->getPos().getX() - 8,
                    this->getPos().getY() - 16 - offset);
      target.draw(f);
    }
  }
  if (shouldHaveSpeechBubble) {
    bubbleSprite_.setPosition(this->getPos().getX() + 2,
                              this->getPos().getY() - 22);
    target.draw(bubbleSprite_);
  }
}

void Character::update(float dtime) {
  setWalking(false);
  if (isControlled()) {
    bool walking = std::abs(getXInput()) > 0.1f || std::abs(getYInput()) > 0.1f;
    if (walking)
      walkToward(
          Vec2(getPos().getX() + getXInput(), getPos().getY() + getYInput()),
          dtime);
    setWalking(walking);
  } else {
    for (GameObject *o : getLevel().getObjects()) {
      if (auto C = dynamic_cast<Creature *>(o)) {
        if (!C->isDead() && isEnemy(*C)) {
          AIAttack(*C, *o, dtime);
          break;
        }
      }
    }
  }
  talking_ = !isControlled();
  if (oldWalkingValue_ != walking_) {
    walkingStartTime_ = getLevel().getTime();
    oldWalkingValue_ = walking_;
  }
}

void Character::equipItem(const Item &i) { equipped_[i.kind()] = i; }

void Character::AIAttack(Creature &C, GameObject &o, float dtime) {
  if (getPos().distance(o.getPos()) < 16) {
    C.damage(2);
  } else {
    walkToward(o.getPos(), dtime);
  }
}

void Character::walkToward(Vec2 pos, float dtime) {
  setWalking(true);
  double angle =
      std::atan2(pos.getY() - getPos().getY(), pos.getX() - getPos().getX());
  float dx = static_cast<float>(std::cos(angle)) * walkSpeed * dtime;
  float dy = static_cast<float>(std::sin(angle)) * walkSpeed * dtime;

  if (dy < 0 && !getLevel().passable(getPos().modY(dy - 3)))
    dy = 0;
  else if (dy > 0 && !getLevel().passable(getPos().modY(dy + 3)))
    dy = 0;
  if (dx < 0 && !getLevel().passable(getPos().modX(dx - 4)))
    dx = 0;
  else if (dx > 0 && !getLevel().passable(getPos().modX(dx + 4)))
    dx = 0;
  setPos(getPos().mod(dx, dy));
}

void Character::setWalking(bool v) {
  if (v != walking_) {
    walking_ = v;
    oldWalkingValue_ = true;
  }
}
