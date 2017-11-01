#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "TilePos.h"

#include <level/Level.h>

class MapSearchNode;

class PathFinder
{
  // Not thread safe due to the A-Star design which requires
  // a default constructor....
  static Level* level_;
  friend class MapSearchNode;
public:
  PathFinder(Level& level);
  void findPath(TilePos start, TilePos end, std::vector<TilePos>& result);
};

#endif // PATHFINDER_H
