#include "WorldGen.h"
#include "LevelGen.h"

WorldGen::WorldGen() {}


static void fillOpenConnections(LevelGen &gen, World &world, GameData &D, unsigned Seed) {
  for (auto &conn : gen.getOpenConnections()) {
    LevelGen gen2(Seed++);
    gen2.generate(world, D, conn.targetType);
    fillOpenConnections(gen2, world, D, Seed * 1000);

    TilePos target = gen2.getBackConnection().pos;

    TilePos out1(conn.pos.getLevel(), conn.pos.getX(), conn.pos.getY() + 1);
    TilePos out2(target.getLevel(), target.getX(), target.getY() - 1);

    target.getLevel().get(target.getX(), target.getY()).setTeleportTarget(out1);
    conn.pos.getLevel()
        .get(conn.pos.getX(), conn.pos.getY())
        .setTeleportTarget(out2);
  }
}


World *WorldGen::generate(GameData &d) {
  World *world = new World(d);

  LevelGen gen;
  gen.generate(*world, d, Level::Type::Overworld);
  fillOpenConnections(gen, *world, d, 55);

  return world;
}
