#include "Level.h"

Level::Level(World &world, Level::Type t, int w, int h, GameData &data)
    : Data_(data), world_(world), type(t) {

  Tile defaultTile(data.tile("void"));
  FloorTiles = TileMap<Tile>(w, h, defaultTile);
  BuildingTiles = TileMap<Tile>(w, h, defaultTile);
  BuildingTiles2 = TileMap<Tile>(w, h, defaultTile);
  OverlayTiles = TileMap<Tile>(w, h, defaultTile);
}

bool Level::getTeleportTarget(TilePos p, TilePos &out) {
  int x = p.getX();
  int y = p.getY();
  if (this->getOverlay(x, y).getTeleportTarget(out))
    return true;
  if (this->getBuilding2(x, y).getTeleportTarget(out))
    return true;
  if (this->getBuilding(x, y).getTeleportTarget(out))
    return true;
  if (this->get(x, y).getTeleportTarget(out))
    return true;
  return false;
}

void Level::render(sf::RenderTarget &target, sf::Vector2f center) {
  int ulx =
      (int)(target.getView().getCenter().x - target.getView().getSize().x / 2) /
      16;
  int uly =
      (int)(target.getView().getCenter().y - target.getView().getSize().y / 2) /
      16;

  int brx =
      (int)(target.getView().getCenter().x + target.getView().getSize().x / 2) /
      16;
  int bry =
      (int)(target.getView().getCenter().y + target.getView().getSize().y / 2) /
      16;

  brx = clampWidth(brx);
  ulx = clampWidth(ulx);
  bry = clampHeight(bry);
  uly = clampHeight(uly);

  for (int x = ulx; x <= brx + 1; x++) {
    for (int y = uly; y <= bry + 1; y++) {
      FloorTiles.get(x, y).render(*this, target, x, y);
    }
  }

  for (int x = ulx; x <= brx + 1; x++) {
    for (int y = uly; y <= bry + 1; y++) {
      BuildingTiles.get(x, y).render(*this, target, x, y);
    }
  }

  for (int x = ulx; x <= brx + 1; x++) {
    for (int y = uly; y <= bry + 1; y++) {
      BuildingTiles2.get(x, y).render(*this, target, x, y);
    }
  }

  for (GameObject *o : Objects)
    o->render(target);

  for (int x = ulx; x <= brx + 1; x++) {
    for (int y = uly; y <= bry + 1; y++) {
      OverlayTiles.get(x, y).render(*this, target, x, y);
    }
  }
}

void Level::update(float dtime) {
  UpdateGuard guard(updating);
  time += dtime;
  for (auto I = Objects.begin(); I != Objects.end();) {
    (*I)->update(dtime);
    if ((*I)->shouldBeRemoved()) {
      delete *I;
      I = Objects.erase(I);
    } else {
      ++I;
    }
  }

  for (auto &o : NewObjects)
    Objects.push_back(o);
  NewObjects.clear();

  std::sort(Objects.begin(), Objects.end(),
            [](const GameObject *O1, const GameObject *O2) {
              return O1->getPos().getY() < O2->getPos().getY();
            });

  ResourceUpdateCounter += dtime;
  if (ResourceUpdateCounter >= ResourceUpdateTime) {
    ResourceUpdateCounter -= ResourceUpdateTime;

    for (Tile &T : BuildingTiles)
      T.updateResources(world_, static_cast<unsigned int>(time));
    for (Tile &T : BuildingTiles2)
      T.updateResources(world_, static_cast<unsigned int>(time));
    for (Tile &T : OverlayTiles)
      T.updateResources(world_, static_cast<unsigned int>(time));
  }
}

void Level::calculateConnections() {
  Connections.clear();
  foreachTilePos([this](int x, int y) {
    TilePos out;
    const TilePos in(*this, x, y);
    if (getTeleportTarget(in, out)) {
      Connections.push_back(LevelConnection(in, out, &out.getLevel()));
    }
    return false; // never stop.
  });
}

void Level::foreachTilePos(std::function<bool(int, int)> lambda) {
  for (int x = 0; x < getWidth(); ++x) {
    for (int y = 0; y < getHeight(); ++y) {
      if (lambda(x, y))
        return;
    }
  }
}

TilePos Level::searchClosestMatchingTile(TilePos Start,
                                         std::function<bool(int, int)> lambda,
                                         int MaxDistance) {

  for (int d = 0; d < MaxDistance; ++d) {
    int StartX = Start.getX() - d;
    StartX = std::max(0, StartX);

    int EndX = Start.getX() + d;
    EndX = std::min(getWidth() - 1, EndX);

    int StartY = Start.getY() - d;
    StartY = std::max(0, StartY);

    int EndY = Start.getY() + d;
    EndY = std::min(getHeight() - 1, EndY);

    for (int x = StartX; x <= EndX; ++x) {
      if (lambda(x, StartY))
        return TilePos(*this, x, StartY);

      if (lambda(x, EndY))
        return TilePos(*this, x, EndY);
    }

    for (int y = StartY; y <= EndY; ++y) {
      if (lambda(StartX, y))
        return TilePos(*this, StartX, y);

      if (lambda(EndX, y))
        return TilePos(*this, EndX, y);
    }
  }
  return TilePos();
}
