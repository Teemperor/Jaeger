#ifndef MAMBO_ITEMGROUP_H
#define MAMBO_ITEMGROUP_H

#include <string>
#include <utility>
#include <utils/WeightedRandomChooser.h>

#include "ItemData.h"

class GameData;

class ItemGroup {

public:
  typedef std::pair<unsigned, std::string> ChoosenItem;

private:
  WeightedRandomChooser<ChoosenItem> Chooser;
  GameData *Data;

public:
  explicit ItemGroup(GameData *D) : Data(D) {}

  void add(unsigned Count, std::string ItemName, unsigned Weight) {
    Chooser.add(std::make_pair(Count, ItemName), Weight);
  }

  std::pair<unsigned, ItemData *> get(unsigned Seed) const;
};

#endif // MAMBO_ITEMGROUP_H
