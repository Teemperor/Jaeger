#include "SellTask.h"

#include <Character.h>

static bool hasItem(Character &C, const std::string &ID) {
  for (auto &I : C.getPrivateInventory())
    if (!I.empty() && I.getID() == ID)
      return true;
  return false;
}

AITask *SellTask::act(Character &C, float DTime) {
  if (ToUnregister == nullptr) {
    ToUnregister = &C;
    ToUnregister->getFaction()->registerFoodOffer(ToUnregister);
  }
  if (!hasItem(C, SellName))
    return finish("Run out of items to sell");
  return nullptr;
}

SellTask::~SellTask() {
  if (ToUnregister) {
    ToUnregister->getFaction()->unregisterFoodOffer(ToUnregister);
  }
}
