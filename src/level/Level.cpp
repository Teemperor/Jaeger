#include "Level.h"

Level::Level(World &world, Level::Type t, int w, int h, GameData &data)
    : Data_(data), world_(world), type(t) {

  Tile defaultTile(data.tile("void"));
  FloorTiles = TileMap<Tile>(w, h, defaultTile);
  BuildingTiles = TileMap<Tile>(w, h, defaultTile);
  OverlayTiles = TileMap<Tile>(w, h, defaultTile);
}

bool Level::getTeleportTarget(TilePos p, TilePos &out) {
  int x = p.getX();
  int y = p.getY();
  if (this->getOverlay(x, y).getTeleportTarget(out))
    return true;
  if (this->getBuilding(x, y).getTeleportTarget(out))
    return true;
  if (this->get(x, y).getTeleportTarget(out))
    return true;
  return false;
}

void Level::render(sf::RenderTarget &target, sf::Vector2f center) {
  int ulx =
      (int)(target.getView().getCenter().x - target.getView().getSize().x / 2) /
      16;
  int uly =
      (int)(target.getView().getCenter().y - target.getView().getSize().y / 2) /
      16;

  int brx =
      (int)(target.getView().getCenter().x + target.getView().getSize().x / 2) /
      16;
  int bry =
      (int)(target.getView().getCenter().y + target.getView().getSize().y / 2) /
      16;

  brx = clampWidth(brx);
  ulx = clampWidth(ulx);
  bry = clampHeight(bry);
  uly = clampHeight(uly);

  for (int x = ulx; x <= brx + 1; x++) {
    for (int y = uly; y <= bry + 1; y++) {
      FloorTiles.get(x, y).render(*this, target, x, y);
    }
  }

  for (int x = ulx; x <= brx + 1; x++) {
    for (int y = uly; y <= bry + 1; y++) {
      BuildingTiles.get(x, y).render(*this, target, x, y);
    }
  }

  for (GameObject *o : Objects)
    o->render(target);

  for (int x = ulx; x <= brx + 1; x++) {
    for (int y = uly; y <= bry + 1; y++) {
      OverlayTiles.get(x, y).render(*this, target, x, y);
    }
  }
}
