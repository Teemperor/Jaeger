#include "Tile.h"

#include "Level.h"

void Tile::render(Level &level, sf::RenderTarget &target, int x, int y) {
  if (!Data)
    return;


  sf::Sprite sprite = Data->sprite(level.timeMillis(), x, y);

  sprite.setPosition(x * 16, y * 16);
  target.draw(sprite);
}
