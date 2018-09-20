#ifndef ARGOS_TILE_H
#define ARGOS_TILE_H

#include <GameObject.h>
#include <chrono>
#include <gamedata/TileData.h>
#include <level/TileExtraInfo.h>

class Level;

class Tile {

  TileData *Data = nullptr;
  TileExtraInfo *Extra = nullptr;

  TileExtraInfo &createExtraInfo();

public:
  Tile() = default;

  explicit Tile(TileData *Data) : Data(Data) {}

  const std::string &name() const {
    if (Data)
      return Data->name();
    else {
      static std::string noName;
      return noName;
    }
  }

  void updateResources(World &W, unsigned Seed) {
    if (isResource())
      createExtraInfo().updateResources(W, *Data, Seed);
  }

  const std::string &group() const {
    if (Data)
      return Data->group();
    else {
      static std::string noGroup;
      return noGroup;
    }
  }

  bool isResource() const { return Data && Data->isResource(); }

  Inventory *getInventory() {
    if (Data && Data->hasInventory()) {
      createExtraInfo().createInventory();
      if (isResource()) {
        createExtraInfo().getInventory().disableItemStorage();
        if (createExtraInfo().getInventory().empty())
          return nullptr;
      }
      return &createExtraInfo().getInventory();
    }
    return nullptr;
  }

  void setTeleportTarget(const TilePos &target);
  bool getTeleportTarget(TilePos &out) const;

  void setData(TileData *Data) { this->Data = Data; }

  bool passable() const {
    if (empty())
      return true;
    return Data->passable();
  }

  bool empty() const { return Data == nullptr; }

  void render(Level &level, sf::RenderTarget &target, int x, int y);
};

#endif // ARGOS_TILE_H
