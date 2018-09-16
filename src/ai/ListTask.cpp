#include "ListTask.h"

AITask *ListTask::act(Character &C, float DTime) {
  if (TasksLeft.empty())
    return finish("No more tasks");
  AITask *Result = TasksLeft.back();
  TasksLeft.pop_back();
  return Result;
}