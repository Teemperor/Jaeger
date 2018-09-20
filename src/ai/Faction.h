#ifndef FACTION_H
#define FACTION_H

#include <unordered_set>

#include <TilePos.h>

class Character;

class Faction {

  std::unordered_set<Character *> OfferingFood;
  std::unordered_set<Faction *> Enemies;
  bool FriendlyByDefault = true;
public:
  explicit Faction(bool Friendly);
  bool isEnemyFaction(Faction &Other) const {
    if (FriendlyByDefault)
      return !Other.isFriendlyToOthers() || Enemies.find(&Other) != Enemies.end();
    else
      return &Other != this;
  }
  bool isFriendlyToOthers() const {
    return FriendlyByDefault;
  }

  void registerFoodOffer(Character *C) {
    OfferingFood.insert(C);
  }
  void unregisterFoodOffer(Character *C) {
    OfferingFood.erase(C);
  }
  Character *closestFoodOffer(TilePos p);
};

#endif // FACTION_H
