#include "AITask.h"

#include "Character.h"
#include "WalkTask.h"
#include "WaitTask.h"
#include "ListTask.h"
#include "FarmerTask.h"

#include <PathFinder.h>
#include <iostream>

AITask *AITask::finish(const char *Reason) {
  // std::cerr << "Finishing Task:" << Reason << std::endl;
  Done = true;
  return nullptr;
}

AITask::AITask() = default;

AITask::~AITask() = default;


class CombatTask : public AITask {
  Creature *Target;

public:
  explicit CombatTask(Creature *Target) : Target(Target) {}

  AITask *act(Character &C, float DTime) override;
};

AITask *CombatTask::act(Character &C, float DTime) {
  if (Target->isDead())
    return finish("Target dead");

  const Item &weapon = C.getEquipped(ItemData::Weapon);

  if (C.getPos().distance(Target->getPos()) > weapon.getRange())
    return finish("Enemy too far away");

  if (&Target->getLevel() != &C.getLevel())
    return finish("Enemey in different level");

  C.tryShootAt(*Target);
  return nullptr;
}

class HuntTask : public AITask {
  Creature *Target;
  float WeaponRange = 0;
  bool InCombat = false;

public:
  explicit HuntTask(Creature *Target) : Target(Target) {}
  AITask *act(Character &C, float DTime) override;

  bool actInactive(Character &C) override {
    if (Target->isDead())
      return true;
    return !InCombat && C.getPos().distance(Target->getPos()) < WeaponRange;
  }
};

AITask *HuntTask::act(Character &C, float DTime) {
  if (Target->isDead())
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

class DefenseTask : public AITask {
  bool First = true;
  TilePos Start;
public:
  AITask *act(Character &C, float DTime) override;
};

AITask *DefenseTask::act(Character &C, float DTime) {
  if (First) {
    Start = C.getTilePos();
    First = false;
  }

  auto ClosestEnemies = C.getClosestEnemies(16 * 20);
  if (!ClosestEnemies.empty()) {
    return new HuntTask(ClosestEnemies.front());
  }
  return new WalkTask(Start);
}

class VillageGuardTask : public AITask {
  bool First = true;
  TilePos House;
  TilePos BeforeHouse;

  std::random_device rd;
  std::mt19937 gen;
  std::normal_distribution<> PosDis;

public:
  VillageGuardTask() : rd(), gen(rd()), PosDis(0, 20) {
  }
  AITask *act(Character &C, float DTime) override;
};

static bool sameLevel(TilePos a, TilePos b) {
  return &a.getLevel() == &b.getLevel();
}

static TilePos getFrontOfHouse(TilePos p) {
  auto &L = p.getLevel();
  for (auto C : L.getConnections()) {
    if (C.getTargetLevel()->getType() == Level::Type::Overworld) {
      return C.getTargetPos();
    }
  }
  return TilePos();
}

AITask *VillageGuardTask::act(Character &C, float DTime) {
  if (First) {
    House = C.getTilePos();
    BeforeHouse = getFrontOfHouse(C.getTilePos()).modY(1);
    First = false;
  }

  if (sameLevel(House, C.getTilePos())) {
    return new WalkTask(BeforeHouse);
  } else if (sameLevel(BeforeHouse, C.getTilePos())) {
    int dx = (int)PosDis(gen);
    int dy = (int)PosDis(gen);

    TilePos Target(BeforeHouse.getLevel(),
                                BeforeHouse.getX() + dx,
                                BeforeHouse.getY() + dy);
    TilePos Out;
    if (C.getLevel().getTeleportTarget(Target, Out))
      return nullptr;
    return new ListTask({new WalkTask(Target), new WaitTask(5)});
  }
  return nullptr;
}

void CharacterAI::popBack() {
  delete Tasks.back();
  Tasks.pop_back();
}

CharacterAI::CharacterAI(Behavior b) {
  switch(b) {
    case Behavior::Farmer:
      Tasks.push_back(new FarmerTask());
      break;
    case Behavior::VillageGuard:
      Tasks.push_back(new VillageGuardTask());
      break;
    case Behavior::Bandit:
      Tasks.push_back(new DefenseTask());
      break;
  }
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
