#ifndef MAMBO_TILERECT_H
#define MAMBO_TILERECT_H

#include <cassert>

class TileRect {

  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;

public:
  TileRect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {
    assert(h > 0);
    assert(w > 0);
  }

  static TileRect fromAbsolute(int x, int y, int rx, int ly) {
    return TileRect(x, y, x - rx, y - ly);
  }

  bool biggerThan(int w, int h) { return this->w > w && this->h > h; }

  TileRect moveX(int v) const {
    TileRect copy = *this;
    copy.x += v;
    return copy;
  }

  TileRect moveY(int v) const {
    TileRect copy = *this;
    copy.y += v;
    return copy;
  }

  TileRect resize(int dw, int dh) {
    TileRect copy = *this;
    copy.w += dw;
    copy.h += dh;
    return copy;
  }

  int getX() const { return x; }

  void setX(int x) { TileRect::x = x; }

  int getY() const { return y; }

  void setY(int y) { TileRect::y = y; }

  int getW() const { return w; }

  void setW(int w) { TileRect::w = w; }

  int getH() const { return h; }

  void setH(int h) { TileRect::h = h; }

  int getRightX() const { return x + w; }

  int getLowerY() const { return y + h; }
};

#endif // MAMBO_TILERECT_H
