#include "Character.h"
#include "PathFinder.h"

#include "level/Level.h"

#include <combat/Projectile.h>

Character::Character(Level &level, Vec2 pos, BodyType type)
    : Creature(level) {
  setBodyType(type);
  bubbleSprite_ = getGameData().getSprite("speech_bubble_exclamation");
  shadow_ = getGameData().getSprite("shadow");
  selection_ = getGameData().getSprite("selection");
  selection_.setScale(1.5f, 1.5f);
  gravestone_ =
      getGameData().getSprite("gravestone" + std::to_string(rand() % 9 + 1));
  setPos(pos);
  equipped_.resize(ItemData::KindLimit);

  auto &W = level.getWorld();
  auto &ID = level.getData();
  addItem(Item(*ID.item("leather_pants"), W));
  addItem(Item(*ID.item("leather_armor"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("red_steel_helmet"), W));
  if (rand() % 2)
    addItem(Item(*ID.item("wood_bow"), W));
  else
    addItem(Item(*ID.item("magic_staff"), W));

  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
  addItem(Item(*ID.item("steel_buckler"), W));
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

///////////////////////////////
// Debug mode for PATHS
#if false
  if (!isDead()) {
    std::vector<sf::Vertex> line;
    for (int i = 0; i < (int)walkPath_.size() - 1; i++) {
      Vec2 currentPos = walkPath_[i];
      Vec2 nextPos = walkPath_[i + 1];
      line.push_back(sf::Vertex(
          sf::Vector2f(currentPos.getX(), currentPos.getY()), sf::Color::Red));
      line.push_back(sf::Vertex(sf::Vector2f(nextPos.getX(), nextPos.getY()),
                                sf::Color::Red));
    }
    target.draw(line.data(), line.size(), sf::Lines);
  }
#endif
  ///////////////////////////////

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

  if (isControlled()) {
    int alpha = (int)(std::abs(std::sin(getLevel().getTime() * 2)) * 100) + 50;

    sf::Color c(255, 0, 0, alpha);
    selection_.setColor(c);
    selection_.setPosition(this->getPos().getX() - 12,
                           this->getPos().getY() - 12);
    target.draw(selection_);
  }

  int offset = 0;
  if (walking_) {
    offset = (int)(std::abs(std::sin(
                       (getLevel().getTime() - walkingStartTime_) * 10)) *
                   4);
  }
  if (isDead()) {
    sf::Sprite &f = gravestone_;
    f.setPosition(this->getPos().getX() - 8, this->getPos().getY() - 16);
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
      sf::Sprite f = i.sprite();
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

namespace {
struct closest_object {
  GameObject *target;
  closest_object(GameObject *target) : target(target) {}

  inline bool operator()(const GameObject *a, const GameObject *b) {
    return a->getPos().distance(target->getPos()) <
           b->getPos().distance(target->getPos());
  }
};
} // namespace

void Character::update(float dtime) {
  Creature::updateEffects(dtime);
  if (isDead()) {
    if (isControlled())
      getControls()->setPossibleTargets({});
    return;
  }
  setWalking(false);

  std::vector<GameObject *> ClosestEnemies;
  for (GameObject *o : getLevel().getObjects()) {
    if (auto C = dynamic_cast<Creature *>(o)) {
      if (!C->isDead() && isEnemy(*C)) {
        ClosestEnemies.push_back(o);
      }
    }
  }
  std::sort(ClosestEnemies.begin(), ClosestEnemies.end(), closest_object(this));

  if (isControlled()) {
    bool walking = std::abs(getXInput()) > 0.1f || std::abs(getYInput()) > 0.1f;
    if (walking)
      walkToward(
          Vec2(getPos().getX() + getXInput(), getPos().getY() + getYInput()),
          dtime);
    setWalking(walking);

    std::vector<GameObject *> PossibleTargets;
    for (auto &e : ClosestEnemies) {
      if (e->getPos().distance(getPos()) <
          equipped_[ItemData::Weapon].getRange()) {
        PossibleTargets.push_back(e);
      }
    }

    getControls()->setPossibleTargets(PossibleTargets);

    if (getControls()->isShooting() && getControls()->getTarget()) {
      tryShootAt(*getControls()->getTarget());
    }
  } else {
    for (auto &e : ClosestEnemies) {
      AIAttack(*dynamic_cast<Creature *>(e), *e, dtime);
      break;
    }
  }

  GameObject::update(dtime);

  talking_ = !isControlled();
  if (oldWalkingValue_ != walking_) {
    walkingStartTime_ = getLevel().getTime();
    oldWalkingValue_ = walking_;
  }
}

void Character::equipItem(const Item &i) { equipped_[i.kind()] = i; }

void Character::AIAttack(Creature &C, GameObject &o, float dtime) {
  Item &weapon = equipped_[ItemData::Weapon];

  if (getPos().distance(o.getPos()) < weapon.getRange()) {
    tryShootAt(o);
    walkPath_.clear();
  } else {
    if (walkPath_.empty()) {
      PathFinder finder(getLevel());
      finder.findPath(getTilePos(), o.getTilePos(), walkPath_);
    } else {
      if (Vec2(walkPath_.back()).distance(getPos()) < 5)
        walkPath_.pop_back();
      walkToward(walkPath_.back(), dtime);
    }
  }
}

void Character::walkToward(Vec2 pos, float dtime, bool backwards) {
  setWalking(true);
  double angle =
      std::atan2(pos.getY() - getPos().getY(), pos.getX() - getPos().getX());
  if (backwards) {
    angle += M_PI;
  }
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

bool Character::tryShootAt(GameObject &o) {
  Item &weapon = equipped_[ItemData::Weapon];
  if (weapon.empty())
    return false;
  if (getPos().distance(o.getPos()) < weapon.getRange()) {
    if (weapon.tryUse(getLevel()) && weapon.hasProjectiles()) {
      new Projectile(*weapon.getProjectileData(), getLevel(), getPos(), *this, o);
      return true;
    }
  }
  return false;
}

void Character::setWalking(bool v) {
  if (v != walking_) {
    walking_ = v;
    oldWalkingValue_ = true;
  }
}
void Character::damage(int dmg) {

  Creature::damage(dmg);
}
