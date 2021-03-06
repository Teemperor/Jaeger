#include "TileData.h"

const sf::Sprite &TileData::sprite(unsigned time, int x, int y) const {
  if (sprites_.empty())
    return noSprite;

  srand((unsigned int)(x + y));

  if (animationTime_ > 0) {
    auto totalTime = animationTime_ * sprites_.size();
    time %= totalTime;
    return sprites_.at(time / animationTime_);
  } else {

    return sprites_.at((x + y) % sprites_.size());
  }
}
