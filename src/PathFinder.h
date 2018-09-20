#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "TilePos.h"

#include <level/Level.h>

class MapSearchNode;

class PathFinder {
  // Not thread safe due to the A-Star design which requires
  // a default constructor....
  friend class MapSearchNode;

  void findPathImpl(TilePos start, TilePos end, bool Adjacent, std::vector<TilePos> &result);

public:
  explicit PathFinder(Level &Level);
  void findPath(TilePos Start, TilePos End, bool Adjacent, std::vector<TilePos> &Result);
};

#endif // PATHFINDER_H
