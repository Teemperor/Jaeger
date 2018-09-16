#include "WaitTask.h"

AITask *WaitTask::act(Character &C, float DTime) {
  TimeLeft -= DTime;
  if (TimeLeft < 0)
    return finish("Wait time left");
  return nullptr;
}