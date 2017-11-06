#include "Level.h"

Level::Level(World &world, Level::Type t, int w, int h, GameData &data)\
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
