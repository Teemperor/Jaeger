#ifndef MAMBO_BUYGROCERIESTASK_H
#define MAMBO_BUYGROCERIESTASK_H

#include "AITask.h"
#include <Creature.h>

class BuyGroceriesTask : public AITask {
  Character *Seller = nullptr;
public:
  AITask *act(Character &C, float DTime) override;
};

#endif //MAMBO_BUYGROCERIESTASK_H
