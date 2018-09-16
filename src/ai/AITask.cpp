#include "AITask.h"

#include "Character.h"

#include <PathFinder.h>
#include <iostream>

AITask *AITask::finish(const char *Reason) {
  // std::cerr << "Finishing Task:" << Reason << std::endl;
  Done = true;
  return nullptr;
}

AITask::AITask() = default;

AITask::~AITask() = default;

class WaitTask : public AITask {

  float TimeLeft;
public:
  explicit WaitTask(float Time) : TimeLeft(Time) {}

  AITask *act(Character &C, float DTime) override;
};

AITask *WaitTask::act(Character &C, float DTime) {
  TimeLeft -= DTime;
  if (TimeLeft < 0)
    return finish("Wait time left");
  return nullptr;
}

class ListTask : public AITask {

  std::vector<AITask *> TasksLeft;
public:
  explicit ListTask(std::initializer_list<AITask *> Tasks) {
    for (auto &T : Tasks)
      TasksLeft.push_back(T);
    std::reverse(TasksLeft.begin(), TasksLeft.end());
  }
  virtual ~ListTask() {
    for (auto &A : TasksLeft)
      delete A;
  }

  AITask *act(Character &C, float DTime) override;
};

AITask *ListTask::act(Character &C, float DTime) {
  if (TasksLeft.empty())
    return finish("No more tasks");
  AITask *Result = TasksLeft.back();
  TasksLeft.pop_back();
  return Result;
}

class WalkTask : public AITask {
  std::vector<TilePos> WalkPath;
  TilePos Target;
  Level *LastLevel = nullptr;

  bool inTargetLevel(Character &C) {
    return &C.getLevel() == &Target.getLevel();
  }
public:
  explicit WalkTask(TilePos Target) : Target(Target) {}

  AITask *act(Character &C, float DTime) override;
};

AITask *WalkTask::act(Character &C, float DTime) {
  if (&C.getLevel() != LastLevel) {
    LastLevel = &C.getLevel();
    WalkPath.clear();
  }

  if (WalkPath.empty()) {
    PathFinder finder(C.getLevel());
    finder.findPath(C.getTilePos(), Target, WalkPath);
  }

  if (WalkPath.empty())
    return finish("Couldn't find a path");

  if (Vec2(WalkPath.back()).distance(C.getPos()) < 5.0f)
    WalkPath.pop_back();

  if (WalkPath.empty()) {
    if (!inTargetLevel(C))
      return finish("Finished path");
  } else
    C.walkToward(WalkPath.back(), DTime);


  return nullptr;
}

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

class FarmerTask : public AITask {
  bool First = true;
  TilePos House;
  TilePos BeforeHouse;
public:
  AITask *act(Character &C, float DTime) override;
};

static TilePos getFrontOfHouse(TilePos p) {
  auto &L = p.getLevel();
  for (auto C : L.getConnections()) {
    if (C.getTargetLevel()->getType() == Level::Type::Overworld) {
      return C.getTargetPos();
    }
  }
  return TilePos();
}

bool sameLevel(TilePos a, TilePos b) {
  return &a.getLevel() == &b.getLevel();
}

AITask *FarmerTask::act(Character &C, float DTime) {
  if (First) {
    House = C.getTilePos();
    BeforeHouse = getFrontOfHouse(C.getTilePos()).modY(1);
    First = false;
  }

  if (sameLevel(House, C.getTilePos())) {
    return new WalkTask(BeforeHouse);
  } else if (sameLevel(BeforeHouse, C.getTilePos())) {
    if (C.getPrivateInventory().full()) {
      return new WalkTask(House);
    } else {
      TilePos nextCorn = C.getLevel().searchClosestMatchingTile(C.getTilePos(),
      [&C](int x, int y) {
        Tile &T = C.getLevel().getBuilding2(x, y);
        if (T.name() == "corn") {
          if (T.getInventory() && !T.getInventory()->empty())
            return true;
        }
        return false;
      }, 20);
      if (nextCorn.valid()) {
        if (nextCorn.distance(C.getTilePos()) <= 2) {
          Inventory *Inv = C.getLevel().getBuilding2(nextCorn.getX(), nextCorn.getY()).getInventory();
          if (Inv)
            C.getPrivateInventory().takeAll(*Inv);
        } else
          return new WalkTask(nextCorn);
      } else
        return new WalkTask(House);
    }
  }
  return nullptr;
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
