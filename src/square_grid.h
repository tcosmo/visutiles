// We operate only on the square grid for our tiles for now
// and below are defined useful characterisations of that grid.
#pragma once

#define WEST_DIR 0
#define SOUTH_DIR 1
#define EAST_DIR 2
#define NORTH_DIR 3

#define WORLD_WEST sf::Vector2i({-1, 0})
#define WORLD_SOUTH sf::Vector2i({0, -1})
#define WORLD_EAST sf::Vector2i({1, 0})
#define WORLD_NORTH \
  sf::Vector2i({0, 1})  // That's not the same convention than SFML

// Cardinal points for the graphic engine
#define VIEW_WEST sf::Vector2f({-1, 0})
#define VIEW_SOUTH sf::Vector2f({0, 1})
#define VIEW_EAST sf::Vector2f({1, 0})
#define VIEW_NORTH sf::Vector2f({0, -1})

#define SQUARE_GRID_DIR \
  { WORLD_WEST, WORLD_SOUTH, WORLD_EAST, WORLD_NORTH }
#define SQUARE_GRID_NEIGHBORING_SIZE 4
