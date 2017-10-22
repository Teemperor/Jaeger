#ifndef ARGOS_TEXTUREATLAS_H
#define ARGOS_TEXTUREATLAS_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <fstream>
#include <Logger.h>
#include <json.hpp>

class TextureAtlas {
  sf::Texture texture;
  std::unordered_map<std::string, sf::Sprite > Sprites;
public:
  TextureAtlas(const std::string& basePath) {
    texture.loadFromFile(basePath + ".png");
    texture.setSmooth(false);
    texture.setRepeated(true);

    nlohmann::json data;

    std::ifstream infile(basePath + ".atlas");

    infile >> data;

    for (auto tex : data["textures"]) {

      sf::Sprite sprite(texture);
      int x = tex["offset"][0];
      int y = tex["offset"][1];
      int wx = tex["size"][0];
      int wy = tex["size"][1];
      sprite.setTextureRect({x, y, wx, wy});
      sprite.setScale({1.01f, 1.01f});

      std::string id = tex["id"];
      Sprites[id] = sprite;
    }
  }

  bool contains(const std::string& id) const {
    return Sprites.find(id) != Sprites.end();
  }

  sf::Sprite& operator[](const std::string& id) {
    return Sprites[id];
  }
};


#endif //ARGOS_TEXTUREATLAS_H
