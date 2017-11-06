#include "LevelGen.h"
#include "WorldGen.h"

WorldGen::WorldGen()
{

}

World *WorldGen::generate(GameData& d) {
  World* world = new World(d);
  {
    LevelGen gen;
    gen.generate(*world, d, Level::Type::Overworld, 200, 200);
  }
  {
    LevelGen gen;
    gen.generate(*world, d, Level::Type::House, 20, 20);
  }
  return world;
}
