#ifndef TILEPOS_H
#define TILEPOS_H

#include <cassert>
#include <initializer_list>

class TilePos {
  int x = 0;
  int y = 0;

public:
  TilePos();
  TilePos(int x, int y) : x(x), y(y) {}
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
};

#endif // TILEPOS_H
