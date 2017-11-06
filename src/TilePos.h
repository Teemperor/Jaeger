#ifndef TILEPOS_H
#define TILEPOS_H

#include <cassert>
#include <initializer_list>

class Vec2;
class Level;

class TilePos {
  int x = 0;
  int y = 0;
  Level* level_ = nullptr;

public:
  TilePos();
  TilePos(const Vec2 &v);
  TilePos(int x, int y) : x(x), y(y) {}
  TilePos(Level& level, int x, int y) : x(x), y(y), level_(&level) {}
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

  int getX() { return x; }
  int getY() { return y; }
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

  bool hasLevel() const {
    return level_ != nullptr;
  }

  Level& getLevel() const {
    assert(level_);
    return *level_;
  }
};

#endif // TILEPOS_H
