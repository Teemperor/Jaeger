#include "GameData.h"
#include <SpellEffect.h>

void GameData::parseItemData(const std::string &path) {
  nlohmann::json data;

  std::ifstream infile(path);

  infile >> data;

  for (auto item : data["items"]) {
    std::string id = item["id"];

    ItemData::Kind kind;
    std::string kindStr = item["kind"];
    if (kindStr == "helmet")
      kind = ItemData::Helmet;
    else if (kindStr == "pants")
      kind = ItemData::Pants;
    else if (kindStr == "armor")
      kind = ItemData::Armor;
    else if (kindStr == "weapon")
      kind = ItemData::Weapon;
    else if (kindStr == "shield")
      kind = ItemData::Shield;
    else if (kindStr == "consumable")
      kind = ItemData::Consumable;
    else {
      std::cerr << "Unknown kind " << kindStr << "\n";
      assert(false);
    }

    Items[id] = new ItemData(kind);

    int attack = 0;
    if (item.find("attack") != item.end()) {
      attack = item["attack"];
    }
    Items[id]->setAttack(attack);

    float cooldown = 0;
    if (item.find("cooldown") != item.end()) {
      cooldown = item["cooldown"];
    }
    Items[id]->setCooldown(cooldown);

    int value = 1;
    if (item.find("value") != item.end()) {
      value = item["value"];
    }
    Items[id]->setValue(value);
    int armor = 0;
    if (item.find("armor") != item.end()) {
      armor = item["armor"];
    }
    Items[id]->setArmor(armor);

    std::string projectile;
    if (item.find("projectile") != item.end()) {
      projectile = item["projectile"];
    }
    Items[id]->setProjectileName(projectile);

    if (item.find("sprite") != item.end()) {
      auto sprite = item["sprite"];
      Items[id]->setSprite(getSprite(sprite));
    }


    if (item.find("effect") != item.end()) {
      std::string EffectName = item["effect"];
      std::string Str = item["effectStr"];
      std::string Dur;
      if (item.find("effectDuration") == item.end()) {
        Dur = "1";
      } else {
        Dur = item["effectDuration"];
      }
      RandomRange EffectStrength(Str);
      RandomRange EffectDuration(Dur);
      const SpellEffect &E = SpellEffects::getByID(EffectName);

      Items[id]->setUseEffect(&E, EffectStrength, EffectDuration);
    }

    if (item.find("icon") != item.end()) {
      Items[id]->setIcon(getSprite(item["icon"]));
    } else {
      Items[id]->setIcon(getSprite("icon_" + id));
    }
  }
}

void GameData::parseProjectileData(const std::string &path) {
  nlohmann::json data;

  std::ifstream infile(path);

  infile >> data;

  for (auto proj : data["projectiles"]) {
    std::string id = proj["id"];

    sf::Sprite sprite = getSprite(proj["sprite"]);
    std::string effectName = proj["effect"];
    std::string str = proj["effectStr"];
    std::string dur;
    if (proj.find("effectDuration") == proj.end()) {
      dur = "1";
    } else {
      dur = proj["effectDuration"];
    }
    RandomRange effectStrength(str);
    RandomRange effectDuration(dur);
    const SpellEffect &E = SpellEffects::getByID(effectName);

    auto P = new ProjectileData(sprite, E, effectStrength, effectDuration);

    if (proj.find("speed") != proj.end()) {
      P->setSpeed(proj["speed"]);
    }

    Projectiles[id] = P;
  }
}

void GameData::parseTileData(const std::string &path) {
  nlohmann::json data;

  std::ifstream infile(path);

  infile >> data;

  for (auto tile : data["tiles"]) {
    std::string id = tile["id"];
    std::string group;

    if (tile.find("group") != tile.end()) {
      group = tile["group"];
    }

    bool passable = true;
    if (tile.find("passable") != tile.end()) {
      passable = tile["passable"];
    }
    int animationTime = -1;
    if (tile.find("animation") != tile.end()) {
      animationTime = tile["animation"];
    }

    Tiles[id] = new TileData(id, group, passable, animationTime);

    if (tile.find("sprites") != tile.end()) {
      auto sprites = tile["sprites"];
      for (const auto &sprite : sprites)
        Tiles[id]->addSprite(getSprite(sprite));
    } else {
      Tiles[id]->addSprite(getSprite(id));
    }
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
    } else if (fileType == "projectiles") {
      mainLogger << "Loading projectile set " << filePath << mainLogger;
      parseProjectileData(filePath);
    } else {
      mainLogger << "Unknown file type in meta file " << path
                 << "/meta.dat: " << mainLogger;
    }
  }
  for (auto &i : Items) {
    i.second->init(*this);
  }
}
