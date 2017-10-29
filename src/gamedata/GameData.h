#ifndef ARGOS_GAMEDATA_H
#define ARGOS_GAMEDATA_H

#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <cassert>
#include <iostream>
#include <Logger.h>
#include "TileData.h"
#include "TextureAtlas.h"
#include "ItemData.h"

class GameData {

  std::vector<TextureAtlas*> TextureAtlas_;
  std::unordered_map<std::string, TileData*> Tiles;
  std::unordered_map<std::string, ItemData*> Items;

  void parseItemData(const std::string& path);

  void parseTileData(const std::string& path);

public:
  GameData(const std::string& path) {
    parseMetaFile(path);

  }

  virtual ~GameData() {
    for (TextureAtlas* s : TextureAtlas_)
      delete s;
    for (auto& s : Tiles)
      delete s.second;
  }

  const sf::Sprite& getSprite(const std::string& id) {
    for (TextureAtlas* ts : TextureAtlas_) {
      if (ts->contains(id)) {
        return (*ts)[id];
      }
    }
    std::cerr << "getSprite(" << id << ")" << std::endl;
    assert(false);
  }

  TileData* tile(const std::string& id) {
    auto I = Tiles.find(id);
    if (I != Tiles.end())
      return I->second;
    std::cerr << "couldn't find " << id << std::endl;
    return nullptr;
  }

  Logger mainLogger;

  void parseMetaFile(const std::string& path);
};


#endif //ARGOS_GAMEDATA_H
