#include "HuntTask.h"
#include "CombatTask.h"
#include "WalkTask.h"

AITask *HuntTask::act(Character &C, float DTime) {
  if (Target->isDead())
    return finish("Target is dead");

  InCombat = false;

  const Item &weapon = C.getEquipped(ItemData::Weapon);
  WeaponRange = weapon.getRange();

  if (C.getPos().distance(Target->getPos()) > WeaponRange) {
    return WalkTask::Beside(Target->getTilePos());
  } else {
    InCombat = true;
    return new CombatTask(Target);
  }
}

bool HuntTask::actInactive(Character &C) {
  if (Target->isDead())
    return true;
  return !InCombat && C.getPos().distance(Target->getPos()) < WeaponRange;
}
