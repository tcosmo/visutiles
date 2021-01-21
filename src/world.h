#pragma once

#include <map>

#include "global.h"

#include "tileset.h"

typedef std::map<sf::Vector2i, TileId, CompareTilesPositions> TileMap;

// The world consists in an assembly (non necessarily connected) of tiles on
// the square grid. Each position can hold exactly one tile.
class World {
 public:
  World(const Tileset& tileset);
  ~World();

  /* Attributes */
  TileMap tile_at_pos;

  const Tileset& tileset;

 private:
};