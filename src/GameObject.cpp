#include "GameObject.h"

#include "level/Level.h"

GameObject::~GameObject() {}

void GameObject::update(float dtime) {
  TilePos currentPos = TilePos(getPos());
  TilePos teleportTarget;
  if (getLevel().getTeleportTarget(currentPos, teleportTarget)) {
    if (teleportTarget.hasLevel()) {
      getLevel().remove(this);
      teleportTarget.getLevel().add(this);
    }
    setPos(Vec2(teleportTarget));
  }
}

GameData &GameObject::getGameData() { return level_->getData(); }
