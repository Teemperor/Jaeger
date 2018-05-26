#ifndef MAMBO_RANDOMRANGE_H
#define MAMBO_RANDOMRANGE_H

#include <random>
#include <string>

class RandomRange {
  int Min = 0;
  int Max = 0;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution;

public:
  RandomRange() = default;
  explicit RandomRange(std::string V);

  int getMin() const { return Min; }

  int getMax() const { return Max; }

  int get();
};

#endif // MAMBO_RANDOMRANGE_H
