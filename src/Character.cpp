#include "Character.h"
#include "PathFinder.h"

#include "level/Level.h"

#include <combat/Projectile.h>

Character::Character(Level &level, Vec2 pos, CharacterAI::Behavior behavior,
                     BodyType type) : Creature(level), MyAI(behavior) {
  setBodyType(type);
  BubbleSprite = getGameData().getSprite("speech_bubble_exclamation");
  Shadow = getGameData().getSprite("shadow");
  Selection = getGameData().getSprite("selection");
  Selection.setScale(1.5f, 1.5f);
  Gravestone =
      getGameData().getSprite("gravestone" + std::to_string(rand() % 9 + 1));
  setPos(pos);
  Equipped.resize(ItemData::KindLimit);
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

    // Render the (facial) hair directly before the helmet is drawn.
    if (layer == ItemData::Kind::Helmet) {
      HairSprite.setPosition(this->getPos().getX() - 8,
                             this->getPos().getY() - 16 - offset);
      target.draw(HairSprite);
    }

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
struct ClosestInventory {
  GameObject *target;
  ClosestInventory(GameObject *target) : target(target) {}

  inline bool operator()(const InventoryLocation &a, const InventoryLocation &b) {
    return a.InvPos.distance(target->getPos()) <
           b.InvPos.distance(target->getPos());
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

  if (!isControlled()) {
    MyAI.act(*this, dtime);
  }

  if (isControlled()) {
    const float InventoryRange = 40;
    std::vector<InventoryLocation> ClosestInventories;
    for (GameObject *o : getLevel().getObjects()) {
      if (o != this && o->getInventory() &&
          o->getPos().distance(getPos()) < InventoryRange) {
        ClosestInventories.push_back({o->getInventory(), o->getPos()});
      }
    }
    for (int x = getTilePos().getX() - 1; x <= getTilePos().getX() + 1; ++x) {
      for (int y = getTilePos().getY() - 1; y <= getTilePos().getY() + 1; ++y) {
        if (auto Inv = getLevel().getInventory(x, y)) {
          ClosestInventories.push_back({Inv, Vec2(TilePos(x, y))});
        }
      }
    }

    std::sort(ClosestInventories.begin(), ClosestInventories.end(),
              ClosestInventory(this));
    getControls()->setPossibleInventoryTargets(ClosestInventories);
  }

  if (isControlled()) {
    bool walking = std::abs(getXInput()) > 0.1f || std::abs(getYInput()) > 0.1f;
    if (walking)
      walkToward(
          Vec2(getPos().getX() + getXInput(), getPos().getY() + getYInput()),
          dtime);
    setWalking(walking);

    std::vector<GameObject *> PossibleTargets;
    auto ClosestEnemies = getClosestEnemies(Equipped[ItemData::Weapon].getRange());
    for (auto &e : ClosestEnemies) {
      PossibleTargets.push_back(e);
    }

    getControls()->setPossibleTargets(PossibleTargets);

    if (getControls()->isShooting() && getControls()->getTarget()) {
      tryShootAt(*getControls()->getTarget());
    }
  }

  Creature::update(dtime);

  //Talking = !isControlled();
  if (OldWalkingValue != Walking) {
    WalkingStartTime = getLevel().getTime();
    OldWalkingValue = Walking;
  }
}

void Character::equipItem(const Item &I) { Equipped[I.kind()] = I; }

void Character::unequipItem(const Item &I) {
  if (equipped(I)) {
    Equipped[I.kind()] = Item();
  }
}

std::vector<Creature *> Character::getClosestEnemies(float distance) {
  std::vector<Creature *> ClosestEnemies;
  for (GameObject *o : getLevel().getObjects()) {
    if (auto C = dynamic_cast<Creature *>(o)) {
      if (!C->isDead() && isEnemy(*C) && C->getPos().distance(getPos()) < distance) {
        ClosestEnemies.push_back(C);
      }
    }
  }

  std::sort(ClosestEnemies.begin(), ClosestEnemies.end(), closest_object(this));
  return ClosestEnemies;
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
  if (getPos().distance(o.getPos()) > weapon.getRange())
    return false;
  if (!trySpendFatigue(weapon.getFatigueCost()))
    return false;
  if (weapon.tryUse(getLevel()) && weapon.hasProjectiles()) {
    new Projectile(*weapon.getProjectileData(), getLevel(), getPos(), *this,
                   o);
    return true;
  }
  return false;
}

void Character::setWalking(bool v) {
  if (v != Walking) {
    Walking = v;
    OldWalkingValue = true;
  }
}
void Character::damage(int Dmg) {
  bool WasDead = isDead();
  Creature::damage(Dmg);
  if (!WasDead && isDead()) {
    onDeath();
  }
}

void Character::useItem(Item &I) {
  if (Inv.contains(I)) {
    addEffect(AppliedSpellEffect(I.getEffect(this)));
    Inv.remove(I);
  }
}

void Character::addItem(const std::string &Name) {
  addItem(Item(*getLevel().getData().item(Name), getLevel().getWorld()));
}

void Character::setHair(const std::string &Name) {
  HairSprite = getLevel().getData().getSprite(Name);
}

