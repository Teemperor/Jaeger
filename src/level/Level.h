#ifndef ARGOS_LEVEL_H
#define ARGOS_LEVEL_H

#include <gamedata/GameData.h>
#include "Tile.h"
#include "TileMap.h"

class Level {

  GameData& Data_;
  std::vector<GameObject *> Objects;

  TileMap<Tile> FloorTiles;
  TileMap<Tile> BuildingTiles;

  size_t w;
  size_t h;

public:
  Level(size_t w, size_t h, GameData& data) : Data_(data), w(w), h(h) {

    FloorTiles = TileMap<Tile>(w, h);
    BuildingTiles = TileMap<Tile>(w, h);

  }


  GameData& getData() {
    return Data_;
  }

  double time = 0;

  Tile& get(int x, int y) {
    return FloorTiles.get(x, y);
  }
  Tile& getBuilding(int x, int y) {
    return BuildingTiles.get(x, y);
  }

  unsigned timeMillis() {
    return (unsigned int) (time * 1000);
  }

  void update() {
    time += 1 / 60.0;
    for (auto I = Objects.begin(); I != Objects.end();) {
      (*I)->update();
      if ((*I)->shouldBeRemoved()) {
        delete *I;
        I = Objects.erase(I);
      } else {
        ++I;
      }
    }
  }

  double getTime() const {
    return time;
  }

  void add(GameObject* o) {
    Objects.push_back(o);
  }

  void render(sf::RenderTarget& target, sf::Vector2f center) {
    int ulx = (int) (target.getView().getCenter().x - target.getView().getSize().x / 2) / 16;
    int uly = (int) (target.getView().getCenter().y - target.getView().getSize().y / 2) / 16;

    int brx = (int) (target.getView().getCenter().x + target.getView().getSize().x / 2) / 16;
    int bry = (int) (target.getView().getCenter().y + target.getView().getSize().y / 2) / 16;


    for (int x = ulx; x <= brx + 1; x++) {
      for (int y = uly; y <= bry + 1; y++) {
        FloorTiles.get(x, y).render(*this, target, x, y);
      }
    }

    for (GameObject* o : Objects)
      o->render(target);

    for (int x = ulx; x <= brx + 1; x++) {
      for (int y = uly; y <= bry + 1; y++) {
        BuildingTiles.get(x, y).render(*this, target, x, y);
      }
    }

  }
};



#endif //ARGOS_LEVEL_H
