#ifndef MAMBO_LEVELCONNECTION_H
#define MAMBO_LEVELCONNECTION_H


#include <TilePos.h>

class Level;

class LevelConnection {
  TilePos Source;
  TilePos Target;
  Level *TargetLevel = nullptr;
public:
  LevelConnection() = default;
  LevelConnection(TilePos Source, TilePos Target, Level *TargetLevel)
    : Source(Source), Target(Target), TargetLevel(TargetLevel) {
  }
  TilePos getSourcePos() const {
    return Source;
  }
  TilePos getTargetPos() const {
    return Target;
  }
  Level *getTargetLevel() const {
    return TargetLevel;
  }
};


#endif //MAMBO_LEVELCONNECTION_H
