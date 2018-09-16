#ifndef FACTION_H
#define FACTION_H

#include <unordered_set>

class Faction {
  std::unordered_set<Faction *> Enemies;
  bool FriendlyByDefault = true;
public:
  Faction(bool Friendly);
  bool isEnemyFaction(Faction &Other) const {
    if (FriendlyByDefault)
      return !Other.isFriendlyToOthers() || Enemies.find(&Other) != Enemies.end();
    else
      return &Other != this;
  }
  bool isFriendlyToOthers() const {
    return FriendlyByDefault;
  }
};

#endif // FACTION_H
