#include "ItemGroup.h"
#include "GameData.h"

std::pair<unsigned int, ItemData *> ItemGroup::get(unsigned Seed) const {
  auto Result = Chooser.get(Seed);

  return std::make_pair(Result.first, Data->item(Result.second));
}
