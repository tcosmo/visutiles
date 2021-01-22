#include "input_factory.h"

TileMap InputFactory::get_initial_configuration() {
  TileMap to_return;

  std::vector<std::pair<sf::Vector2i, TileId>> tiles_to_add;
  tiles_to_add.push_back({{0, 0}, 0});
  tiles_to_add.push_back({{0, 1}, 1});
  tiles_to_add.push_back({{1, 0}, 2});
  tiles_to_add.push_back({{1, 1}, 3});
  tiles_to_add.push_back({{-1, 0}, 4});
  tiles_to_add.push_back({{-2, 0}, 5});

  for (int i = 0; i < 20000; i += 1) {
    tiles_to_add.push_back({{2, i}, 5});
  }

  for (const std::pair<sf::Vector2i, TileId>& pos_and_tile : tiles_to_add) {
    to_return[pos_and_tile.first] = pos_and_tile.second;
  }

  return to_return;
}

TileMap CollatzInputFactory::build_parity_vector_initial_configuration() {
  TileMap to_return;

  bool i_have_met_a_one = false;

  sf::Vector2i curr_pos = {0, 0};

  WangColor north_constraint, east_constraint;

  east_constraint = 1;  // Wang color of binary 0
  for (char c : input_str) {
    if (!(c == '0' || c == '1')) {
      fatal_error_log(
          "A Collatz parity vector must contain only 0s and 1s, not the case "
          "in `%s`. Abort.",
          input_str.c_str());
    }

    if (c == '0' && !i_have_met_a_one) {
      curr_pos += WORLD_WEST;
      continue;
    }

    if (c == '1') {
      if (i_have_met_a_one) {
        std::vector<TileId> query_result = CollatzTileset.Wang_query(
            {-1, -1, east_constraint, north_constraint});
        assert(query_result.size() == 1);
        to_return[curr_pos] = query_result[0];

        north_constraint =
            CollatzTileset.tiles[query_result[0]].wang_edges[SOUTH_DIR];
      }

      curr_pos += WORLD_SOUTH;
      north_constraint = 3;  // Wang color of ternary 1
      i_have_met_a_one = true;
    }

    std::vector<TileId> query_result =
        CollatzTileset.Wang_query({-1, -1, east_constraint, north_constraint});
    assert(query_result.size() == 1);
    to_return[curr_pos] = query_result[0];

    north_constraint =
        CollatzTileset.tiles[query_result[0]].wang_edges[SOUTH_DIR];

    curr_pos += WORLD_WEST;
  }

  return to_return;
}

TileMap CollatzInputFactory::get_initial_configuration() {
  TileMap to_return;

  if (input_type == COLLATZ_PARITY_VECTOR) {
    to_return = std::move(build_parity_vector_initial_configuration());
  }

  return to_return;
}