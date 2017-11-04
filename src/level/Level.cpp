#include "Level.h"

Level::Level(World &world, Level::Type t, int w, int h, GameData &data)\
  : Data_(data), world_(world), type(t) {

  FloorTiles = TileMap<Tile>(w, h);
  BuildingTiles = TileMap<Tile>(w, h);
  OverlayTiles = TileMap<Tile>(w, h);
}
