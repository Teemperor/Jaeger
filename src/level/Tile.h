#ifndef ARGOS_TILE_H
#define ARGOS_TILE_H


#include <GameObject.h>
#include <gamedata/TileData.h>
#include <chrono>

class Level;

class Tile {

  TileData* Data = nullptr;

public:
  Tile() {}

  const std::string& name() const {
    if (Data)
      return Data->name();
    else {
      static std::string noName;
      return noName;
    }
  }

  void setData(TileData* Data) {
    this->Data = Data;
  }

  bool passable() const {
    if(empty())
      return true;
    return Data->passable();
  }

  bool empty() const {
    return !Data;
  }

  void render(Level& level, sf::RenderTarget& target, int x, int y);

};



#endif //ARGOS_TILE_H
