#include "TilePos.h"
#include "Vec2.h"

TilePos::TilePos() {}

TilePos::TilePos(const Vec2 &v) {
  X = v.getX() / 16;
  Y = v.getY() / 16;
}
