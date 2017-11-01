#include "TilePos.h"
#include "Vec2.h"

TilePos::TilePos() {}

TilePos::TilePos(const Vec2 &v) {
  x = v.getX() / 16;
  y = v.getY() / 16;
}
