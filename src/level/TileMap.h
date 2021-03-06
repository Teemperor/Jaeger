#ifndef ARGOS_TILEMAP_H
#define ARGOS_TILEMAP_H

#include <cstddef>
#include <vector>

template <typename T> class TileMap {

  std::vector<T> tiles;

  int width_, height_;

  T defaultTile;

public:
  TileMap() {}
  TileMap(int w, int h, T defaultTile)
      : width_(w), height_(h), defaultTile(defaultTile) {
    tiles.resize(w * h);
  }

  T &get(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
      return defaultTile;
    }
    return tiles[x + y * width_];
  }

  int width() const { return width_; }

  int height() const { return height_; }

  T *begin() { return tiles.data(); }

  T *end() { return tiles.data() + tiles.size(); }
};

#endif // ARGOS_TILEMAP_H
