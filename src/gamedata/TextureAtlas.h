#ifndef ARGOS_TEXTUREATLAS_H
#define ARGOS_TEXTUREATLAS_H

#include <Logger.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <json.hpp>
#include <unordered_map>

class TextureAtlas {
  sf::Texture texture;
  std::unordered_map<std::string, sf::Sprite> Sprites;

public:
  TextureAtlas(const std::string &basePath) {
    texture.loadFromFile(basePath + ".png");
    texture.setSmooth(false);
    texture.setRepeated(true);

    nlohmann::json data;

    std::ifstream infile(basePath + ".json");

    infile >> data;

    for (auto tex : data["textures"]) {

      sf::Sprite sprite(texture);
      int x, y;
      if (tex.find("offset") != tex.end()) {
        x = tex["offset"][0];
        y = tex["offset"][1];
      } else {
        x = tex["tile"][0];
        y = tex["tile"][1];
        x *= 17;
        y *= 17;
      }
      int wx = 16, wy = 16;
      if (tex.find("size") != tex.end()) {
        wx = tex["size"][0];
        wy = tex["size"][1];
      }
      sprite.setTextureRect({x, y, wx, wy});
      sprite.setScale({1.01f, 1.01f});

      int ox = 0, oy = 0;
      if (tex.find("center") != tex.end()) {
        ox = tex["center"][0];
        oy = tex["center"][1];
        sprite.setOrigin(ox, oy);
      }

      std::string id = tex["id"];
      Sprites[id] = sprite;
    }
  }

  bool contains(const std::string &id) const {
    return Sprites.find(id) != Sprites.end();
  }

  sf::Sprite &operator[](const std::string &id) { return Sprites[id]; }
};

#endif // ARGOS_TEXTUREATLAS_H
