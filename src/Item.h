#ifndef ITEM_H
#define ITEM_H

#include "gamedata/ItemData.h"

class Item
{
  ItemData* Data_ = nullptr;

public:
  Item(ItemData& Data);
  Item() {}

  bool empty() const {
    return Data_ == nullptr;
  }

  sf::Sprite& sprite() {
    return Data_->sprite();
  }

  ItemData::Kind kind() const {
    return Data_->kind();
  }

  int armor() const {
    return Data_->armor();
  }

  int attack() const {
    return Data_->attack();
  }

  int value() const {
    return Data_->value();
  }
};

#endif // ITEM_H
