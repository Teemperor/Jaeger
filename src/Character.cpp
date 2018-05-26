#include "Character.h"
#include "PathFinder.h"

#include "level/Level.h"

#include <combat/Projectile.h>

Character::Character(Level &level, Vec2 pos, BodyType type) : Creature(level) {
  setBodyType(type);
  BubbleSprite = getGameData().getSprite("speech_bubble_exclamation");
  Shadow = getGameData().getSprite("shadow");
  Selection = getGameData().getSprite("selection");
  Selection.setScale(1.5f, 1.5f);
  Gravestone =
      getGameData().getSprite("gravestone" + std::to_string(rand() % 9 + 1));
  setPos(pos);
  Equipped.resize(ItemData::KindLimit);

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
  TheBodyType = t;
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
  Sprite = getGameData().getSprite(SpriteName);
  TalkingSprite = getGameData().getSprite(SpriteName + "_talking");
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
  if (Talking) {
    if (getLevel().getTimeModulo(0.4) > 0.2 ||
        getLevel().getTimeModulo(2.5) > 2.1) {
      shouldUseTalkingSprite = true;
    }
    if (getLevel().getTimeModulo(3) > 0.6) {
      shouldHaveSpeechBubble = true;
    }
  }

  Shadow.setPosition(this->getPos().getX() - 8, this->getPos().getY() - 2);
  target.draw(Shadow);

  if (isControlled()) {
    int alpha = (int)(std::abs(std::sin(getLevel().getTime() * 2)) * 100) + 50;

    sf::Color c(255, 0, 0, alpha);
    Selection.setColor(c);
    Selection.setPosition(this->getPos().getX() - 12,
                          this->getPos().getY() - 12);
    target.draw(Selection);
  }

  int offset = 0;
  if (Walking) {
    offset = (int)(std::abs(std::sin((getLevel().getTime() - WalkingStartTime) *
                                     10)) *
                   4);
  }
  if (isDead()) {
    sf::Sprite &f = Gravestone;
    f.setPosition(this->getPos().getX() - 8, this->getPos().getY() - 16);
    target.draw(f);
    return;
  } else {
    sf::Sprite &f = shouldUseTalkingSprite ? TalkingSprite : Sprite;
    f.setPosition(this->getPos().getX() - 8,
                  this->getPos().getY() - 16 - offset);
    target.draw(f);
  }
  for (unsigned layer = 0; layer < Equipped.size(); layer++) {
    Item &i = Equipped.at(layer);
    if (!i.empty()) {
      sf::Sprite f = i.sprite();
      f.setPosition(this->getPos().getX() - 8,
                    this->getPos().getY() - 16 - offset);
      target.draw(f);
    }
  }
  if (shouldHaveSpeechBubble) {
    BubbleSprite.setPosition(this->getPos().getX() + 2,
                             this->getPos().getY() - 22);
    target.draw(BubbleSprite);
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
          Equipped[ItemData::Weapon].getRange()) {
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

  Talking = !isControlled();
  if (OldWalkingValue != Walking) {
    WalkingStartTime = getLevel().getTime();
    OldWalkingValue = Walking;
  }
}

void Character::equipItem(const Item &i) { Equipped[i.kind()] = i; }

void Character::AIAttack(Creature &C, GameObject &o, float dtime) {
  Item &weapon = Equipped[ItemData::Weapon];

  if (getPos().distance(o.getPos()) < weapon.getRange()) {
    tryShootAt(o);
    WalkPath.clear();
  } else {
    if (WalkPath.empty()) {
      PathFinder finder(getLevel());
      finder.findPath(getTilePos(), o.getTilePos(), WalkPath);
    } else {
      if (Vec2(WalkPath.back()).distance(getPos()) < 5)
        WalkPath.pop_back();
      walkToward(WalkPath.back(), dtime);
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
  float dx = static_cast<float>(std::cos(angle)) * WalkSpeed * dtime;
  float dy = static_cast<float>(std::sin(angle)) * WalkSpeed * dtime;

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
  Item &weapon = Equipped[ItemData::Weapon];
  if (weapon.empty())
    return false;
  if (getPos().distance(o.getPos()) < weapon.getRange()) {
    if (weapon.tryUse(getLevel()) && weapon.hasProjectiles()) {
      new Projectile(*weapon.getProjectileData(), getLevel(), getPos(), *this,
                     o);
      return true;
    }
  }
  return false;
}

void Character::setWalking(bool v) {
  if (v != Walking) {
    Walking = v;
    OldWalkingValue = true;
  }
}
void Character::damage(int dmg) { Creature::damage(dmg); }
