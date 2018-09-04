#ifndef ARGOS_TILEDATA_H
#define ARGOS_TILEDATA_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "ItemGroup.h"

class TileData {

  std::vector<sf::Sprite> sprites_;
  std::string name_;
  std::string group_;
  bool passable_ = false;
  int animationTime_ = -1;

  bool HasInventory = false;
  bool IsResource = false;
  std::string Resources;
  unsigned ResourceCounter = 1;

  sf::Sprite noSprite;

public:
  TileData(const std::string &name, const std::string &group, bool passable,
           int animationTime)
      : name_(name), group_(group), passable_(passable),
        animationTime_(animationTime) {
    if (group_.empty())
      group_ = name;
  }

  const std::string &name() const { return name_; }

  const std::string &group() const { return group_; }

  std::string getResources() const {
    return Resources;
  }

  void setResources(const std::string &I) {
    enableInventory();
    Resources = I;
    IsResource = true;
  }

  unsigned int getResourceCounter() const {
    return ResourceCounter;
  }

  bool isResource() const {
    return IsResource;
  }

  void enableInventory() {
    HasInventory = true;
  }

  bool hasInventory() const {
    return HasInventory;
  }

  void passable(bool v) { passable_ = v; }

  bool passable() { return passable_; }

  void addSprite(const sf::Sprite &s) { sprites_.push_back(s); }

  const sf::Sprite &sprite(unsigned time, int x, int y) const;
};

#endif // ARGOS_TILEDATA_H
