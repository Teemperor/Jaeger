#include "Vec2.h"

#include "TilePos.h"

Vec2::Vec2() {}

Vec2::Vec2(TilePos &p) {
  x = p.getX() * 16 + 8;
  y = p.getY() * 16 + 8;
}
