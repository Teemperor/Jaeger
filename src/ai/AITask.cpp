#include "AITask.h"

#include "Character.h"
#include "CombatTask.h"
#include "DefenseTask.h"
#include "FarmerTask.h"
#include "FarmerWifeTask.h"
#include "HuntTask.h"
#include "ListTask.h"
#include "VillageGuardTask.h"
#include "WaitTask.h"

#include <PathFinder.h>
#include <iostream>

AITask *AITask::finish(const char *Reason) {
  // std::cerr << "Finishing Task:" << Reason << std::endl;
  Done = true;
  return nullptr;
}

AITask::AITask() = default;

AITask::~AITask() = default;

void CharacterAI::popBack() {
  delete Tasks.back();
  Tasks.pop_back();
}

CharacterAI::CharacterAI(Behavior b) {
  switch (b) {
  case Behavior::Farmer:
    Tasks.push_back(new FarmerTask());
    break;
  case Behavior::FarmerWife:
    Tasks.push_back(new FarmerWifeTask());
    break;
  case Behavior::VillageGuard:
    Tasks.push_back(new VillageGuardTask());
    break;
  case Behavior::Bandit:
    Tasks.push_back(new DefenseTask());
    break;
  }
}

void CharacterAI::act(Character &C, float DTime) {
  if (!Tasks.empty()) {
    AITask *NewTask = Tasks.back()->act(C, DTime);

    if (Tasks.back()->done())
      popBack();

    if (NewTask)
      Tasks.push_back(NewTask);
  }
  for (auto &T : Tasks) {
    if (T->actInactive(C)) {
      while (&Tasks.back() != &T) {
        popBack();
      }
      break;
    }
  }
}

void CharacterAI::debugDraw(Character &C, sf::RenderTarget &target) {
  if (!Tasks.empty()) {
    Tasks.back()->debugDraw(C, target);
  }
}
