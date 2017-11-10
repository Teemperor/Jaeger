#include "ItemData.h"

#include "GameData.h"

void ItemData::init(GameData &data) {
  if (!projectileName_.empty()) {
    projectileData_ = data.projectile(projectileName_);
  }
}
