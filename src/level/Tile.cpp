#include "Tile.h"

#include "Level.h"

TileExtraInfo &Tile::createExtraInfo() {
  if (!Extra)
    Extra = new TileExtraInfo();
  return *Extra;
}

void Tile::setTeleportTarget(const TilePos &target) {
  createExtraInfo().setTeleportTarget(target);
}

bool Tile::getTeleportTarget(TilePos &out) const {
  if (Extra) {
    out = Extra->getTeleportTarget();
    return Extra->isTeleporting();
  }
  return false;
}

void Tile::render(Level &level, sf::RenderTarget &target, int x, int y) {
  if (empty())
    return;
  if (isResource() && (!getInventory() || getInventory()->empty()))
    return;

  sf::Sprite sprite = Data->sprite(level.timeMillis(), x, y);

  sprite.setPosition(x * 16, y * 16);
  target.draw(sprite);
}
