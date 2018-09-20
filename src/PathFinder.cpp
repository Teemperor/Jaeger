#include "PathFinder.h"

#include "stlastar.h"
#include <cmath>
#include <level/Level.h>

struct SearchInfo {
  Level *TheLevel = nullptr;
  bool Adjacent = false;
  TilePos Goal;
};
static thread_local SearchInfo CurrentSearchInfo;

class MapSearchNode {
public:
  int x; // the (x,y) positions of the node
  int y;

  MapSearchNode() { x = y = 0; }
  MapSearchNode(int px, int py) {
    x = px;
    y = py;
  }

  float GoalDistanceEstimate(MapSearchNode &nodeGoal);
  bool IsGoal(MapSearchNode &nodeGoal);
  bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
                     MapSearchNode *parent_node);
  float GetCost(MapSearchNode &successor);
  bool IsSameState(MapSearchNode &rhs);

  void PrintNodeInfo();
  void MakeSuccessor(AStarSearch<MapSearchNode> *astarsearch,
                     int parent_x, int parent_y, int dx, int dy) {
    const int tx = x + dx;
    const int ty = y + dy;
    if (!isFree(tx, ty))
      return;
    if (parent_x == tx + dx && parent_y == ty)
      return;
    // Diagnoals need all 4 involved tiles to be free.
    if (dx != 0 && dy != 0) {
      if (!isFree(x, ty) || !isFree(tx, y))
        return;
    }
    MapSearchNode NewNode = MapSearchNode(tx, ty);
    astarsearch->AddSuccessor(NewNode);
  }

  bool isFree(int x, int y) {
    TilePos T(*CurrentSearchInfo.TheLevel, x, y);
    if (CurrentSearchInfo.Adjacent && T == CurrentSearchInfo.Goal)
      return true;
    return CurrentSearchInfo.TheLevel->passable(T);
  }
};

bool MapSearchNode::IsSameState(MapSearchNode &rhs) {
  return (x == rhs.x) && (y == rhs.y);
}

void MapSearchNode::PrintNodeInfo() {
  char str[100];
  sprintf(str, "Node position : (%d,%d)\n", x, y);

  cout << str;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal.

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal) {
  return std::abs(x - nodeGoal.x) + std::abs(y - nodeGoal.y);
}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal) {
  return (x == nodeGoal.x) && (y == nodeGoal.y);
}

// This generates the successors to the given Node. It uses a helper function
// called AddSuccessor to give the successors to the AStar class. The A*
// specific initialisation is done for each node internally, so here you just
// set the state information that is specific to the application
bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
                                  MapSearchNode *parent_node) {

  int parent_x = -100;
  int parent_y = -100;

  if (parent_node) {
    parent_x = parent_node->x;
    parent_y = parent_node->y;
  }

  MapSearchNode NewNode;

  // push each possible move except allowing the search to go backwards

  MakeSuccessor(astarsearch, parent_x, parent_y, -1, 0);
  MakeSuccessor(astarsearch, parent_x, parent_y,  1, 0);
  MakeSuccessor(astarsearch, parent_x, parent_y,  0, -1);
  MakeSuccessor(astarsearch, parent_x, parent_y,  0,  1);
  // Diagonals
  MakeSuccessor(astarsearch, parent_x, parent_y, -1, -1);
  MakeSuccessor(astarsearch, parent_x, parent_y,  1,  1);
  MakeSuccessor(astarsearch, parent_x, parent_y,  1, -1);
  MakeSuccessor(astarsearch, parent_x, parent_y, -1,  1);

  return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is
// conceptually where we're moving

float MapSearchNode::GetCost(MapSearchNode &successor) { return 1; }

PathFinder::PathFinder(Level &Level) {}

void PathFinder::findPathImpl(TilePos start, TilePos end, bool Adjacent,
                              std::vector<TilePos> &result) {
  CurrentSearchInfo.TheLevel = &start.getLevel();
  CurrentSearchInfo.Adjacent = Adjacent;
  CurrentSearchInfo.Goal = end;

  // Our sample problem defines the world as a 2d array representing a terrain
  // Each element contains an integer from 0 to 5 which indicates the cost
  // of travel across the terrain. Zero means the least possible difficulty
  // in travelling (think ice rink if you can skate) whilst 5 represents the
  // most difficult. 9 indicates that we cannot pass.

  // Create an instance of the search class...

  AStarSearch<MapSearchNode> astarsearch;

  // Create a start state
  MapSearchNode nodeStart(start.getX(), start.getY());

  // Define the goal state
  MapSearchNode nodeEnd(end.getX(), end.getY());

  // Set Start and goal states

  astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

  unsigned int SearchState;
  unsigned int SearchSteps = 0;

  do {
    SearchState = astarsearch.SearchStep();
    SearchSteps++;
  } while (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

  if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED) {

    MapSearchNode *node = astarsearch.GetSolutionStart();
    int steps = 0;

    // node->PrintNodeInfo();
    for (;;) {
      node = astarsearch.GetSolutionNext();

      if (!node) {
        break;
      }
      result.emplace_back(*CurrentSearchInfo.TheLevel, node->x, node->y);
      // node->PrintNodeInfo();
      steps++;
    }
    std::reverse(result.begin(), result.end());

    while (!result.empty() && !result.back().getLevel().passable(result.back())) {
      result.pop_back();
    }

    // cout << "Solution steps " << steps << endl;

    // Once you're done with the solution you can free the nodes up
    astarsearch.FreeSolutionNodes();

  } else if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED) {
    // cout << "Search terminated. Did not find goal state\n";
  }

  // Display the number of loops the search went through
  // cout << "SearchSteps : " << SearchSteps << "\n";
  //
  astarsearch.EnsureMemoryFreed();
}

void PathFinder::findPath(TilePos Start, TilePos End, bool Adjacent,
                          std::vector<TilePos> &Result) {
  if (&Start.getLevel() == &End.getLevel()) {
    findPathImpl(Start, End, Adjacent, Result);
    if (Adjacent && !Result.empty()) {
      assert(End != Result.back() && "Not adjacent, but on the target?");
    }
    return;
  }
  for (auto &c : Start.getLevel().getConnections()) {
    if (c.getTargetLevel() == &End.getLevel()) {
      findPath(Start, c.getSourcePos(), false, Result);
      return;
    }
  }
}
