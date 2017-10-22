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

  void parseItemData(const std::string& path) {
    nlohmann::json data;

    std::ifstream infile(path);

    infile >> data;

    for (auto item : data["items"]) {
      std::string id = item["id"];
      Items[id] = new ItemData();

      int attack = 0;
      if (item.find("attack") != item.end()) {
        attack = item["attack"];
      }
      Items[id]->setAttack(attack);
      int value = 0;
      if (item.find("value") != item.end()) {
        value = item["value"];
      }
      Items[id]->setValue(value);
      int armor = 0;
      if (item.find("armor") != item.end()) {
        armor = item["armor"];
      }
      Items[id]->setArmor(armor);


      auto sprite = item["sprite"];
      Items[id]->setSprite(getSprite(sprite));
    }
  }

  void parseTileData(const std::string& path) {
    nlohmann::json data;

    std::ifstream infile(path);

    infile >> data;

    for (auto tile : data["tiles"]) {
      std::string id = tile["id"];

      bool passable = true;
      if (tile.find("passable") != tile.end()) {
        passable = tile["passable"];
      }
      int animationTime = -1;
      if (tile.find("animation") != tile.end()) {
        animationTime = tile["animation"];
      }

      Tiles[id] = new TileData(id, passable, animationTime);

      auto sprites = tile["sprites"];
      for (auto sprite : sprites)
        Tiles[id]->addSprite(getSprite(sprite));
    }
  }

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

  void parseMetaFile(const std::string& path) {
    mainLogger << "Parsing " << path << "/meta.dat" << mainLogger;
    std::ifstream input(path + "/meta.dat");
    std::string fileType;
    std::string filePath;
    while (input >> fileType >> filePath) {
      filePath = path + "/" + filePath;
      if (fileType == "meta") {
        parseMetaFile(filePath);
      } else if (fileType == "texatlas") {
        mainLogger << "Loading texture atlas " << filePath << mainLogger;
        TextureAtlas_.push_back(new TextureAtlas(filePath));
      } else if (fileType == "tiles") {
        mainLogger << "Loading tile set " << filePath << mainLogger;
        parseTileData(filePath);
      } else if (fileType == "items") {
        mainLogger << "Loading item set " << filePath << mainLogger;
        parseItemData(filePath);
      } else {
        mainLogger << "Unknown file type in meta file " << path << "/meta.dat: "
                   << mainLogger;
      }
    }
  }
};


#endif //ARGOS_GAMEDATA_H
