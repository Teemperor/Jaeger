#ifndef TILEPOS_H
#define TILEPOS_H

#include <cassert>
#include <initializer_list>
#include <tuple>

class Vec2;
class Level;

class TilePos {

  Level *level_ = nullptr;
  int x = 0;
  int y = 0;

public:
  TilePos();
  TilePos(const Vec2 &v);
  TilePos(int x, int y) : x(x), y(y) {}
  TilePos(Level &level, int x, int y) : level_(&level), x(x), y(y) {}
  TilePos(std::initializer_list<int> l) {
    std::size_t elements = 0;
    for (auto e : l) {
      switch (elements) {
      case 0:
        x = e;
        break;
      case 1:
        y = e;
        break;
      default:
        assert("too many elements" && false);
      }
      elements++;
    }
  }

  int getX() const { return x; }
  int getY() const { return y; }
  TilePos modX(int d) {
    x += d;
    return *this;
  }
  TilePos modY(int d) {
    y += d;
    return *this;
  }
  int setX(int v) { return x = v; }
  int setY(int v) { return y = v; }
  TilePos mod(int xd, int yd) {
    x += xd;
    y += yd;
    return *this;
  }

  bool operator<(const TilePos &o) const {
    return std::tie(x, y, level_) < std::tie(o.x, o.y, o.level_);
  }

  bool hasLevel() const { return level_ != nullptr; }

  Level &getLevel() const {
    assert(level_);
    return *level_;
  }
};

#endif // TILEPOS_H
