#include "GameObject.h"

#include "level/Level.h"

GameObject::~GameObject() {
}

GameData &GameObject::getGameData() {
  return level_->getData();
}
