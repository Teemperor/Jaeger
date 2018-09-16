#ifndef MAMBO_WAITTASK_H
#define MAMBO_WAITTASK_H


#include "AITask.h"

class WaitTask : public AITask {

  float TimeLeft;
public:
  explicit WaitTask(float Time) : TimeLeft(Time) {}

  AITask *act(Character &C, float DTime) override;
};


#endif //MAMBO_WAITTASK_H
