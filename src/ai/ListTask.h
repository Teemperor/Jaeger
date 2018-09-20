#ifndef MAMBO_LISTTASK_H
#define MAMBO_LISTTASK_H

#include "AITask.h"
#include <algorithm>

class ListTask : public AITask {

  std::vector<AITask *> TasksLeft;

public:
  explicit ListTask(std::initializer_list<AITask *> Tasks) {
    for (auto &T : Tasks)
      TasksLeft.push_back(T);
    std::reverse(TasksLeft.begin(), TasksLeft.end());
  }
  virtual ~ListTask() {
    for (auto &A : TasksLeft)
      delete A;
  }

  AITask *act(Character &C, float DTime) override;
};

#endif // MAMBO_LISTTASK_H
