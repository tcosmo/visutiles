#pragma once

#include <map>
#include <set>
#include <vector>

#include "global.h"

#define WORLD_EAST sf::Vector2i({1, 0})
#define WORLD_WEST sf::Vector2i({-1, 0})
#define WORLD_SOUTH sf::Vector2i({0, -1})
#define WORLD_NORTH \
  sf::Vector2i({0, 1})  // That's not the same convention than SFML

#define SQUARE_GRID_NEIGHBORING_SIZE 4
#define SQUARE_GRID_DIR \
  { WORLD_NORTH, WORLD_EAST, WORLD_SOUTH, WORLD_WEST }

// The world consists in an assembly (non necessarily connected) of tiles on
// the square grid. Each position can hold exactly one tile.
class World {
 public:
  World();
  ~World();

  /* Attributes */
  std::map<sf::Vector2i, int, CompareTilesPositions> tiles;

 private:
};