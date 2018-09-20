#include "ai/Faction.h"

#include <level/Level.h>
#include <Character.h>

Faction::Faction(bool Friendly) : FriendlyByDefault(Friendly) {}

Character *Faction::closestFoodOffer(TilePos p) {
  float distance = std::numeric_limits<float>::max();
  Character *Result = nullptr;
  for (Character *C : OfferingFood) {
    float d = C->getTilePos().distance(p);
    if (d < distance) {
      distance = d;
      Result = C;
    }
  }
  return Result;
}
