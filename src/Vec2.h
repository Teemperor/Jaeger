#ifndef VEC2_H
#define VEC2_H

#include <initializer_list>
#include <cassert>

class Vec2
{
    float x = 0;
    float y = 0;
public:
  Vec2();
  Vec2(std::initializer_list<float> l) {
      std::size_t elements = 0;
      for(auto e : l) {
        switch(elements) {
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

  float getX() {
      return x;
  }
  float getY() {
      return y;
  }
  float modX(float d) {
      return x += d;
  }
  float modY(float d) {
      return y += d;
  }
  float setX(float v) {
      return x = v;
  }
  float setY(float v) {
      return y = v;
  }
  Vec2 mod(float xd, float yd) {
    x += xd;
    y += yd;
    return *this;
  }
};

#endif // VEC2_H
