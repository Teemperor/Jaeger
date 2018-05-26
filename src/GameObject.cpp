#include "GameObject.h"

#include "level/Level.h"
#include "level/World.h"

GameObject::GameObject(Level &l) : TheLevel(&l) { l.add(this); }

GameObject::~GameObject() {}

void GameObject::update(float dtime) {
  TilePos currentPos = TilePos(getPos());
  TilePos teleportTarget;
  if (getLevel().getTeleportTarget(currentPos, teleportTarget)) {
    if (teleportTarget.hasLevel()) {
      TheLevel->getWorld().queryTeleport(this, teleportTarget);
    } else {
      setPos(Vec2(teleportTarget));
    }
  }
}

GameData &GameObject::getGameData() { return TheLevel->getData(); }
