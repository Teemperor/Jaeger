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

  TileMap<Tile> FloorTiles;
  TileMap<Tile> BuildingTiles;
  // Renders the objects
  TileMap<Tile> OverlayTiles;
  World &world_;

public:
  enum class Type {
    Overworld,
    Cave,
    House
  };
private:
  Type type;
  
public:
  Level(World& world, Type t, int w, int h, GameData &data);

  GameData &getData() { return Data_; }

  double time = 0;

  bool passable(TilePos pos) {
    int x = pos.getX();
    int y = pos.getY();
    if (!get(x, y).passable())
      return false;
    if (!getBuilding(x, y).passable())
      return false;
    return true;
  }

  bool passable(Vec2 pos) {
    int x = (int)pos.getX();
    x /= 16;
    int y = (int)pos.getY();
    y /= 16;
    if (!get(x, y).passable())
      return false;
    if (!getBuilding(x, y).passable())
      return false;
    return true;
  }

  int getWidth() const {
    return FloorTiles.width();
  }

  int getHeight() const {
    return FloorTiles.height();
  }

  Tile &get(int x, int y) { return FloorTiles.get(x, y); }
  Tile &getBuilding(int x, int y) { return BuildingTiles.get(x, y); }
  Tile &getOverlay(int x, int y) { return OverlayTiles.get(x, y); }

  Type getType() const {
    return type;
  }

  unsigned timeMillis() { return static_cast<unsigned>(time * 1000); }

  void update(float dtime) {
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
  }

  std::vector<GameObject *> &getObjects() { return Objects; }

  double getTime() const { return time; }

  double getTimeModulo(double modulo) {
    unsigned i = static_cast<unsigned>(modulo * 1000);
    unsigned timeInt = static_cast<unsigned>(time * 1000);
    return (timeInt % i) / 1000.0;
  }

  void add(GameObject *o) { Objects.push_back(o); }

  void render(sf::RenderTarget &target, sf::Vector2f center) {
    int ulx = (int)(target.getView().getCenter().x -
                    target.getView().getSize().x / 2) /
              16;
    int uly = (int)(target.getView().getCenter().y -
                    target.getView().getSize().y / 2) /
              16;

    int brx = (int)(target.getView().getCenter().x +
                    target.getView().getSize().x / 2) /
              16;
    int bry = (int)(target.getView().getCenter().y +
                    target.getView().getSize().y / 2) /
              16;

    for (int x = ulx; x <= brx + 1; x++) {
      for (int y = uly; y <= bry + 1; y++) {
        FloorTiles.get(x, y).render(*this, target, x, y);
      }
    }

    for (int x = ulx; x <= brx + 1; x++) {
      for (int y = uly; y <= bry + 1; y++) {
        BuildingTiles.get(x, y).render(*this, target, x, y);
      }
    }

    for (GameObject *o : Objects)
      o->render(target);

    for (int x = ulx; x <= brx + 1; x++) {
      for (int y = uly; y <= bry + 1; y++) {
        OverlayTiles.get(x, y).render(*this, target, x, y);
      }
    }
  }
};

#endif // ARGOS_LEVEL_H
