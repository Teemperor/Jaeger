#ifndef PERLIN2D_H
#define PERLIN2D_H

#include "stb_perlin.h"

#include <random>

class Perlin2D {
  float Scale;
  std::mt19937 Gen;
  std::uniform_real_distribution<float> Dis;
  float Height = 0;

public:
  Perlin2D() = default;
  Perlin2D(unsigned seed, float Scale) : Scale(Scale), Gen(seed), Dis(0, 1) {
    Height = Dis(Gen) * 256;
  }
  float get(float x, float y) {
    float result = stb_perlin_noise3(Scale * x, Scale * y, Height) +
                   stb_perlin_noise3(Scale * x, Scale * y, -Height);
    if (result > 1)
      return 1;
    if (result < -1)
      return -1;
    return result;
  }
};

#endif // PERLIN2D_H
