#ifndef ARGOS_GAMEDATA_H
#define ARGOS_GAMEDATA_H

#include "ItemData.h"
#include "ProjectileData.h"
#include "TextureAtlas.h"
#include "TileData.h"
#include <Logger.h>
#include <SFML/Graphics/Sprite.hpp>
#include <cassert>
#include <iostream>
#include <vector>

class GameData {

  std::vector<TextureAtlas *> TextureAtlas_;
  std::unordered_map<std::string, TileData *> Tiles;
  std::unordered_map<std::string, ItemData *> Items;
  std::unordered_map<std::string, ProjectileData *> Projectiles;

  void parseItemData(const std::string &path);
  void parseProjectileData(const std::string &path);

  void parseTileData(const std::string &path);

public:
  GameData(const std::string &path) { parseMetaFile(path); }

  virtual ~GameData() {
    for (TextureAtlas *s : TextureAtlas_)
      delete s;
    for (auto &s : Tiles)
      delete s.second;
  }

  sf::Sprite getSprite(const std::string &id) {
    for (TextureAtlas *ts : TextureAtlas_) {
      if (ts->contains(id)) {
        return (*ts)[id];
      }
    }
    return sf::Sprite();
  }

  TileData *tile(const std::string &id) {
    auto I = Tiles.find(id);
    if (I != Tiles.end())
      return I->second;
    std::cerr << "couldn't find " << id << std::endl;
    return nullptr;
  }

  ItemData *item(const std::string &id) {
    auto I = Items.find(id);
    if (I != Items.end())
      return I->second;
    std::cerr << "couldn't find " << id << std::endl;
    return nullptr;
  }

  ProjectileData *projectile(const std::string &id) {
    auto I = Projectiles.find(id);
    if (I != Projectiles.end())
      return I->second;
    std::cerr << "couldn't find " << id << std::endl;
    return nullptr;
  }

  Logger mainLogger;

  void parseMetaFile(const std::string &path);
};

#endif // ARGOS_GAMEDATA_H
