#ifndef TILEPOS_H
#define TILEPOS_H

#include <cassert>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <tuple>

class Vec2;
class Level;

class TilePos {

  Level *TheLevel = nullptr;
  int X = 0;
  int Y = 0;

public:
  TilePos();
  TilePos(const Vec2 &v);
  TilePos(int x, int y) : X(x), Y(y) {}
  TilePos(Level &level, int x, int y) : TheLevel(&level), X(x), Y(y) {}
  TilePos(std::initializer_list<int> L) {
    std::size_t Elements = 0;
    for (auto Elem : L) {
      switch (Elements) {
      case 0:
        X = Elem;
        break;
      case 1:
        Y = Elem;
        break;
      default:
        assert("too many elements" && false);
      }
      Elements++;
    }
  }

  int getX() const { return X; }
  int getY() const { return Y; }
  TilePos modX(int D) {
    X += D;
    return *this;
  }
  TilePos modY(int D) {
    Y += D;
    return *this;
  }
  int setX(int V) { return X = V; }
  int setY(int V) { return Y = V; }
  TilePos mod(int xd, int yd) {
    X += xd;
    Y += yd;
    return *this;
  }

  float distance(const TilePos &Other) {
    if (&Other.getLevel() != &getLevel())
      return std::numeric_limits<float>::max();
    int dx = X - Other.X;
    int dy = Y - Other.Y;
    return static_cast<float>(std::sqrt(dx * dx + dy * dy));
  }

  bool operator<(const TilePos &o) const {
    return std::tie(X, Y, TheLevel) < std::tie(o.X, o.Y, o.TheLevel);
  }

  bool operator==(const TilePos &o) const {
    return std::tie(X, Y, TheLevel) == std::tie(o.X, o.Y, o.TheLevel);
  }

  bool operator!=(const TilePos &o) const {
    return std::tie(X, Y, TheLevel) != std::tie(o.X, o.Y, o.TheLevel);
  }

  bool hasLevel() const { return TheLevel != nullptr; }

  Level &getLevel() const {
    assert(TheLevel);
    return *TheLevel;
  }

  bool valid() const { return hasLevel(); }
};

#endif // TILEPOS_H
