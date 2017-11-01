#ifndef FACTION_H
#define FACTION_H

class Faction {
public:
  Faction();
  bool isEnemyFaction(Faction &Other) { return &Other != this; }
};

#endif // FACTION_H
