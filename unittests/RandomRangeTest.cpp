

#include <gamedata/RandomRange.h>
#include <cassert>
#include <iostream>

void test(RandomRange R, int Min, int Max) {
  for (unsigned i = 0; i < 10000; ++i) {
    auto Val = R.get();
    if (Val < Min || Val > Max) {
      std::cerr << "Wrong value " << Val << std::endl;
      assert(false);
    }
  }
}

int main() {
  assert(RandomRange("1:2").getMin() == 1);
  assert(RandomRange("1:2").getMax() == 2);
  test(RandomRange("1:2"), 1, 2);

  assert(RandomRange("1:1").getMin() == 1);
  assert(RandomRange("1:1").getMax() == 1);
  test(RandomRange("1:1"), 1, 1);

  assert(RandomRange("1").getMin() == 1);
  assert(RandomRange("1").getMax() == 1);
  test(RandomRange("1"), 1, 1);
}
