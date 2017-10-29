#include "GameData.h"

void GameData::parseItemData(const std::string &path) {
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

void GameData::parseTileData(const std::string &path) {
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

void GameData::parseMetaFile(const std::string &path) {
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
