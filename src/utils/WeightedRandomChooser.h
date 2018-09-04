#ifndef MAMBO_WEIGHTEDRANDOMCHOOSER_H
#define MAMBO_WEIGHTEDRANDOMCHOOSER_H

#include <vector>
#include <random>
#include <cassert>

template<typename T>
class WeightedRandomChooser {
  std::vector<std::pair<T, unsigned>> Data;
  unsigned WeightSum = 0;
public:
  void add(T D, unsigned Weight) {
    Data.push_back(std::make_pair(D, Weight));
    WeightSum += Weight;
    assert(Weight > 0);
  }

  const T &get(std::size_t Seed) const {
    assert(WeightSum && "Can't chooose from empty RandomChooser");
    std::mt19937 Gen(Seed);
    std::uniform_int_distribution<unsigned> Dis(0, WeightSum - 1);
    unsigned R = Dis(Gen);
    for (auto &P : Data) {
      if (R < P.second) {
        return P.first;
      } else {
        R -= P.second;
      }
    }
    assert(false);
    return Data.back().first;
  }

  bool empty() {
    return WeightSum == 0;
  }
};


#endif //MAMBO_WEIGHTEDRANDOMCHOOSER_H
