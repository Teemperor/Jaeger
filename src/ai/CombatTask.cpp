#include "CombatTask.h"

#include <Character.h>

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