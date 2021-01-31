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

// This function returns the coordinates of the tiles separated by a give edge
// on the square grid
static std::array<sf::Vector2i, 2> square_grid_tiles_pos_from_edge_pos(
    const OrderedPosCouple& pos_couple) {
  sf::Vector2i vect = pos_couple.get_vector();

  if (vect == WORLD_NORTH) {
    return {pos_couple.first, pos_couple.first + WORLD_EAST};
  } else if (vect == WORLD_EAST) {
    return {pos_couple.second, pos_couple.second + WORLD_SOUTH};
  } else {
    fatal_error_log(
        "An invalid edge was given on the square grid. Here's the edges's "
        "vector: (%d,%d). Abort.\n",
        vect.x, vect.y);
  }
}

// Returns the set of edges for a tile position on the square grid in order
// West, South, East, North
static std::array<OrderedPosCouple, 4> square_grid_edges_pos_from_tile_pos(
    const sf::Vector2i& tile_pos) {
  return {OrderedPosCouple(tile_pos + WORLD_WEST,
                           tile_pos + WORLD_WEST + WORLD_NORTH),
          OrderedPosCouple(tile_pos, tile_pos + WORLD_WEST),
          OrderedPosCouple(tile_pos, tile_pos + WORLD_NORTH),
          OrderedPosCouple(tile_pos + WORLD_NORTH + WORLD_WEST,
                           tile_pos + WORLD_NORTH)};
}