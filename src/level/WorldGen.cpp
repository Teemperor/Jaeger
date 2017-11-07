#include "WorldGen.h"
#include "LevelGen.h"

WorldGen::WorldGen() {}

World *WorldGen::generate(GameData &d) {
  World *world = new World(d);

  LevelGen gen;
  gen.generate(*world, d, Level::Type::Overworld);

  for (auto &conn : gen.getOpenConnections()) {
    LevelGen gen2;
    gen2.generate(*world, d, conn.targetType);
    TilePos target = gen2.getOpenConnections().front().pos;

    TilePos out1(conn.pos.getLevel(), conn.pos.getX(), conn.pos.getY() + 1);
    TilePos out2(target.getLevel(), target.getX(), target.getY() - 1);

    target.getLevel().get(target.getX(), target.getY()).setTeleportTarget(out1);
    conn.pos.getLevel()
        .get(conn.pos.getX(), conn.pos.getY())
        .setTeleportTarget(out2);
  }
  return world;
}
