#include "RandomRange.h"

#include <regex>
#include <iostream>
#include <cassert>
#include <random>

RandomRange::RandomRange(std::string V) {
  std::regex NumRegex("[0-9]+");
  std::smatch m;
  if (std::regex_match(V, m, NumRegex)) {
    Min = Max = std::atoi(V.c_str());
  } else {
    std::regex RangeRegex("([0-9]+):([0-9]+)");
    if (std::regex_match(V, m, RangeRegex)) {
      assert(m.size() == 3);
      Min = std::atoi(m[1].str().c_str());
      Max = std::atoi(m[2].str().c_str());
    }
  }
  distribution = std::uniform_int_distribution<int>(Min,Max);
}

int RandomRange::get() {
  return distribution(generator);
}
