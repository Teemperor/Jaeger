#ifndef WORLD_H
#define WORLD_H

#include "Level.h"
#include <ai/Faction.h>

#include <vector>

class GameObject;

class World {
  std::vector<Level *> levels_;
  GameData &Data;
  std::vector<std::pair<GameObject *, TilePos>> ToTeleport;

  uint64_t ItemId = 0;

  std::vector<Faction *> Factions;

public:
  explicit World(GameData &d);
  void update(float dtime);

  uint64_t getFreshItemID() { return ++ItemId; }

  void queryTeleport(GameObject *o, TilePos target) {
    ToTeleport.emplace_back(o, target);
  }

  void addFaction(Faction *F) { Factions.push_back(F); }

  GameData &getGameData() { return Data; }

  void addLevel(Level *level) { levels_.push_back(level); }

  std::vector<Level *> &getLevels() { return levels_; }
};

#endif // WORLD_H
