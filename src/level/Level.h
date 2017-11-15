#ifndef ARGOS_LEVEL_H
#define ARGOS_LEVEL_H

#include "GameObject.h"
#include "Tile.h"
#include "TileMap.h"
#include <TilePos.h>
#include <gamedata/GameData.h>

class World;

class Level {

  GameData &Data_;
  std::vector<GameObject *> Objects;
  std::vector<GameObject *> NewObjects;

  TileMap<Tile> FloorTiles;
  TileMap<Tile> BuildingTiles;
  // Renders the objects
  TileMap<Tile> OverlayTiles;
  World &world_;

  bool updating = false;

  struct UpdateGuard {
    bool &flag;
    UpdateGuard(bool &flag) : flag(flag) { flag = true; }
    ~UpdateGuard() { flag = false; }
  };

public:
  enum class Type { Overworld, Cave, House };

private:
  Type type;

  int clampWidth(int x) {
    if (x < 0)
      return 0;
    if (x >= getWidth())
      return getWidth() - 1;
    return x;
  }

  int clampHeight(int x) {
    if (x < 0)
      return 0;
    if (x >= getHeight())
      return getHeight() - 1;
    return x;
  }

public:
  Level(World &world, Type t, int w, int h, GameData &data);

  GameData &getData() { return Data_; }

  double time = 0;

  bool passable(TilePos pos) {
    int x = pos.getX();
    int y = pos.getY();
    if (!getOverlay(x, y).passable())
      return false;
    if (!getBuilding(x, y).passable())
      return false;
    if (!get(x, y).passable())
      return false;
    return true;
  }

  bool passable(Vec2 pos) {
    int x = (int)pos.getX();
    x /= 16;
    int y = (int)pos.getY();
    y /= 16;
    if (!getOverlay(x, y).passable())
      return false;
    if (!getBuilding(x, y).passable())
      return false;
    if (!get(x, y).passable())
      return false;
    return true;
  }

  bool getTeleportTarget(TilePos p, TilePos &out);

  int getWidth() const { return FloorTiles.width(); }

  int getHeight() const { return FloorTiles.height(); }

  Tile &get(int x, int y) { return FloorTiles.get(x, y); }
  Tile &getBuilding(int x, int y) { return BuildingTiles.get(x, y); }
  Tile &getOverlay(int x, int y) { return OverlayTiles.get(x, y); }

  Type getType() const { return type; }

  World &getWorld() { return world_; }

  unsigned timeMillis() { return static_cast<unsigned>(time * 1000); }

  void update(float dtime) {
    UpdateGuard guard(updating);
    time += dtime;
    for (auto I = Objects.begin(); I != Objects.end();) {
      (*I)->update(dtime);
      if ((*I)->shouldBeRemoved()) {
        delete *I;
        I = Objects.erase(I);
      } else {
        ++I;
      }
    }
    for (auto &o : NewObjects)
      Objects.push_back(o);
    NewObjects.clear();
  }

  std::vector<GameObject *> &getObjects() { return Objects; }

  double getTime() const { return time; }

  double getTimeModulo(double modulo) {
    unsigned i = static_cast<unsigned>(modulo * 1000);
    unsigned timeInt = static_cast<unsigned>(time * 1000);
    return (timeInt % i) / 1000.0;
  }

  void add(GameObject *o) {
    if (!updating)
      Objects.push_back(o);
    else
      NewObjects.push_back(o);
  }

  void remove(GameObject *o) {
    auto it = std::find(Objects.begin(), Objects.end(), o);
    assert(it != Objects.end());
    Objects.erase(it);
  }

  void render(sf::RenderTarget &target, sf::Vector2f center);
};

#endif // ARGOS_LEVEL_H
