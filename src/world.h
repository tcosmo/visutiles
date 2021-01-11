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

// Dictates the behavior the world's evolution
enum GrowthMode {
  // Tiles are added one by one in a default order (lexicographic)
  ONE_ORDERED,

  // All tiles that can be added on a given configuration are added at once
  MAXIMALLY_BATCHED
};

// The world consists in an assembly (non necessarily connected) of tiles on
// the square grid. Each position can hold exactly one tile.
class World {
 public:
  World(bool allow_non_deterministic_attachments = false,
        GrowthMode growth_mode = MAXIMALLY_BATCHED);
  ~World();

  /* Attributes */

  // Can tiles attach if two different tiles fit at a position?
  bool allow_non_deterministic_attachments;

  // Defines in what way growth steps are performed.
  GrowthMode growth_mode;

  std::map<sf::Vector2i, int, CompareTilesPositions> tiles;

  // This vector temporary stores which tiles where added last.
  // Useful for computing the assembly's edge and communicating with
  // graphic engine.
  PosVec newest_tiles_pos;

  /* Methods */

  // Perform one step of assembly growth. How coarse grained these steps are
  // depends on  the attribute `growth_mode`
  void grow();

 private:
  /* Methods */

  PosVec get_neighbouring_tiles_pos(const sf::Vector2i& pos);
};