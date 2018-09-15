#include "AITask.h"

#include "Character.h"

#include <iostream>
#include <PathFinder.h>

AITask *AITask::finish(const char *Reason) {
  std::cerr << "Finishing Task:" << Reason << std::endl;
  Done = true;
  return nullptr;
}

AITask::AITask() {
}

AITask::~AITask() {
}

class WalkTask : public AITask {
  std::vector<TilePos> WalkPath;
  TilePos Target;
public:
  WalkTask(TilePos Target) : Target(Target) {
    std::cerr << "Creating WalkTask" << std::endl;
  }
  ~WalkTask() override {
    std::cerr << "Destroying WalkTask" << std::endl;
  }

  AITask *act(Character &C, float DTime) override {
    if (WalkPath.empty()) {
      PathFinder finder(C.getLevel());
      finder.findPath(C.getTilePos(), Target, WalkPath);
    }

    if (WalkPath.empty())
      return finish("Couldn't find a path");

    if (Vec2(WalkPath.back()).distance(C.getPos()) < 5.0f)
      WalkPath.pop_back();

    if (WalkPath.empty())
      return finish("Finished path");

    C.walkToward(WalkPath.back(), DTime);

    return nullptr;
  }
};

class CombatTask : public AITask {
  Creature *Target;
public:
  CombatTask(Creature *Target) : Target(Target) {
    std::cerr << "Creating CombatTask" << std::endl;
  }
  ~CombatTask() override {
    std::cerr << "Destroying CombatTask" << std::endl;
  }

  AITask *act(Character &C, float DTime) override {
    if (C.isDead())
      return finish("Target dead");

    const Item &weapon = C.getEquipped(ItemData::Weapon);

    if (C.getPos().distance(Target->getPos()) > weapon.getRange())
      return finish("Enemy too far away");

    if (&Target->getLevel() != &C.getLevel())
      return finish("Enemey in different level");

    C.tryShootAt(*Target);
    return nullptr;
  }
};

class HuntTask : public AITask {
  Creature *Target;
  float WeaponRange = 0;
  bool InCombat = false;
public:
  HuntTask(Creature *Target) : Target(Target) {
    std::cerr << "Creating HuntTask" << std::endl;
  }
  AITask *act(Character &C, float DTime) override {
    if (C.isDead())
      return finish("Target is dead");

    InCombat = false;

    const Item &weapon = C.getEquipped(ItemData::Weapon);
    WeaponRange = weapon.getRange();

    if (C.getPos().distance(Target->getPos()) > WeaponRange) {
      return new WalkTask(Target->getTilePos());
    } else {
      InCombat = true;
      return new CombatTask(Target);
    }
  }

  bool actInactive(Character &C) override {
    return !InCombat && C.getPos().distance(Target->getPos()) < WeaponRange;
  }
};

class MainTask : public AITask {
public:
  AITask *act(Character &C, float DTime) override {
    auto ClosestEnemies = C.getClosestEnemies(16 * 20);
    if (!ClosestEnemies.empty()) {
      return new HuntTask(ClosestEnemies.front());
    }
    return nullptr;
  }
};

void CharacterAI::popBack() {
  delete Tasks.back();
  Tasks.pop_back();
}

CharacterAI::CharacterAI() {
  Tasks.push_back(new MainTask());
}

void CharacterAI::act(Character &C, float DTime) {
  if (!Tasks.empty()) {
    AITask *NewTask = Tasks.back()->act(C, DTime);

    if (Tasks.back()->done())
      popBack();

    if (NewTask)
      Tasks.push_back(NewTask);
  }
  for (auto &T : Tasks) {
    if (T->actInactive(C)) {
      while (&Tasks.back() != &T) {
        popBack();
      }
      break;
    }
  }
}
