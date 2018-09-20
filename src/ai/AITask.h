#ifndef AITASK_H
#define AITASK_H

#include <vector>

class Character;

namespace sf { class RenderTarget; }

class AITask {
protected:
  bool Done = false;
  AITask *finish(const char *Reason);

public:
  AITask();
  virtual ~AITask();
  virtual AITask *act(Character &C, float DTime) = 0;
  virtual bool actInactive(Character &C) { return false; }
  virtual void debugDraw(Character &C, sf::RenderTarget &target) {}

  bool done() const { return Done; }
};

class CharacterAI {
  std::vector<AITask *> Tasks;
  void popBack();

public:
  enum class Behavior { Farmer, FarmerWife, VillageGuard, Bandit };
  explicit CharacterAI(Behavior b);

  void act(Character &C, float DTime);
  void debugDraw(Character &C, sf::RenderTarget &target);
};

#endif // AITASK_H
