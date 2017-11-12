#ifndef VEC2_H
#define VEC2_H

#include <cassert>
#include <cmath>
#include <initializer_list>

class TilePos;

class Vec2 {
  float x = 0;
  float y = 0;

public:
  Vec2();
  Vec2(const TilePos &p);
  Vec2(float x, float y) : x(x), y(y) {}
  Vec2(std::initializer_list<float> l) {
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

  float distance(const Vec2 &other) {
    float dx = other.x - x;
    float dy = other.y - y;
    return std::sqrt(dx * dx + dy * dy);
  }

  float getX() const { return x; }
  float getY() const { return y; }
  Vec2 modX(float d) {
    x += d;
    return *this;
  }
  Vec2 modY(float d) {
    y += d;
    return *this;
  }
  float setX(float v) { return x = v; }
  float setY(float v) { return y = v; }
  Vec2 mod(float xd, float yd) {
    x += xd;
    y += yd;
    return *this;
  }
  Vec2 mod(const Vec2 &o) {
    x += o.x;
    y += o.y;
    return *this;
  }

  Vec2 operator-(const Vec2 &other) const {
    return Vec2(x - other.x, y - other.y);
  }
  Vec2 operator+(const Vec2 &other) const {
    return Vec2(x + other.x, y + other.y);
  }
  Vec2 operator*(float f) const { return Vec2(x * f, y * f); }
};

#endif // VEC2_H
