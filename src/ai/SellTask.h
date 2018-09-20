#ifndef MAMBO_SELLTASK_H
#define MAMBO_SELLTASK_H


#include <Creature.h>
#include "AITask.h"

class SellTask : public AITask {
  std::string SellName = "corn";
  Character *ToUnregister = nullptr;
public:
  explicit SellTask() {}
  ~SellTask() override;

  AITask *act(Character &C, float DTime) override;
};

#endif //MAMBO_SELLTASK_H
