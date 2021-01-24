#include "world.h"

World::World(Tileset& tileset) : tileset(tileset) {}

// This function returns the coordinates of the tiles separated by a give edge
// on the square grid
std::array<sf::Vector2i, 2> square_grid_tiles_from_edge(
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

// Returns the set of edges for a tile on the square grid in order
// West, South, East, North
std::array<OrderedPosCouple, 4> square_grid_edges_from_tile(
    const sf::Vector2i& tile_pos) {
  return {OrderedPosCouple(tile_pos + WORLD_WEST,
                           tile_pos + WORLD_WEST + WORLD_NORTH),
          OrderedPosCouple(tile_pos, tile_pos + WORLD_WEST),
          OrderedPosCouple(tile_pos, tile_pos + WORLD_NORTH),
          OrderedPosCouple(tile_pos + WORLD_NORTH + WORLD_WEST,
                           tile_pos + WORLD_NORTH)};
}
void World::print_all_edges() {
  for (const PosEdge& edge : edges) {
    printf("\t(%d,%d) (%d,%d) (%s,%d)\n", edge.first.first.x,
           edge.first.first.y, edge.first.second.x, edge.first.second.y,
           edge.second.first.c_str(), edge.second.second);
  }
}
void World::set_edges(EdgeMap p_edges) {
  edges = std::move(p_edges);
  print_all_edges();

  for (const PosEdge& pos_edge : edges) {
    newly_added_edges.push_back(pos_edge);
  }

  // Insert tiles corresponding to edges
  add_tiles_for_newly_added_edges();
}

void World::add_tiles_for_newly_added_edges() {
  for (const PosEdge& pos_edge : newly_added_edges) {
    for (const sf::Vector2i& tile_pos :
         square_grid_tiles_from_edge(pos_edge.first)) {
      uncompleted_tiles.insert(tile_pos);
    }
  }

  remove_completed_and_dead_tiles();
}
// Add the edges of tile specification `tile_name` at position `tile_pos`
void World::add_edges_for_tile(const sf::Vector2i& tile_pos,
                               TileName tile_name) {
  std::array<OrderedPosCouple, 4> edges_pos_of_tile =
      square_grid_edges_from_tile(tile_pos);

  size_t i_dir = 0;
  for (const OrderedPosCouple& edge_pos : edges_pos_of_tile) {
    if (edges.find(edge_pos) == edges.end()) {
      edges[edge_pos] = tileset.get_tile_spec(tile_name)[i_dir];

      newly_added_edges.push_back(std::make_pair(edge_pos, edges[edge_pos]));
    } else {
      assert(edges[edge_pos] ==
             tileset.get_tile_spec(tile_name)[i_dir]);  // asset no mismatches
    }
    i_dir += 1;
  }
}

// World update step: add edges of tiles that can be determined from their
// current constraints
void World::update() {
  newly_added_edges.clear();

  std::vector<sf::Vector2i> to_remove;
  for (const sf::Vector2i& tile_pos : uncompleted_tiles) {
    printf("(%d, %d):\n", tile_pos.x, tile_pos.y);
    std::vector<TileName> Wang_query_result = Wang_query_for_tile_pos(tile_pos);
    printf("%d\n", Wang_query_result.size());
    if (Wang_query_result.size() > 1) continue;

    if (Wang_query_result.size() == 0) {
      dead_tiles.push_back(tile_pos);
    } else {
      add_edges_for_tile(tile_pos, Wang_query_result[0]);
      completed_tiles[tile_pos] = Wang_query_result[0];
    }

    to_remove.push_back(tile_pos);
  }

  for (const sf::Vector2i& tile_pos : to_remove) {
    uncompleted_tiles.erase(tile_pos);
  }

  add_tiles_for_newly_added_edges();
  printf("\n\n");
}

// Decides if a given tiles has all its edges
bool World::is_tile_completed(const sf::Vector2i& tile_pos) {
  for (const OrderedPosCouple& edge : square_grid_edges_from_tile(tile_pos)) {
    if (edges.find(edge) == edges.end()) return false;
  }

  return true;
}

std::vector<TileName> World::Wang_query_for_tile_pos(
    const sf::Vector2i& tile_pos) {
  // for (const PosEdge& pos_edge : edges) {
  //   printf("Edge (%d %d) (%d %d) (%s %d)\n", pos_edge.first.first.x,
  //          pos_edge.first.first.y, pos_edge.first.second.x,
  //          pos_edge.first.second.y, pos_edge.second.first.c_str(),
  //          pos_edge.second.second);
  // }

  std::array<EdgeColor, 4> Wang_constraint;
  int i_edge = 0;
  for (const OrderedPosCouple& edge : square_grid_edges_from_tile(tile_pos)) {
    Wang_constraint[i_edge] = ANY_EDGE_COLOR;
    if (edges.find(edge) != edges.end()) {
      Wang_constraint[i_edge] = edges[edge];
    }
    printf("\t(%d,%d) (%d,%d) (%s,%d)\n", edge.first.x, edge.first.y,
           edge.second.x, edge.second.y, Wang_constraint[i_edge].first.c_str(),
           Wang_constraint[i_edge].second);
    i_edge += 1;
  }
  return tileset.Wang_query(Wang_constraint);
}

// This routine goes through the uncompleted tiles
// and remove the completed or deads ones (i.e. tiles with all their edges)
void World::remove_completed_and_dead_tiles() {
  PosVec to_remove;
  for (const sf::Vector2i& tile_pos : uncompleted_tiles) {
    if (is_tile_completed(tile_pos)) {
      to_remove.push_back(tile_pos);
    }
  }
  for (const sf::Vector2i& tile_pos : to_remove) {
    std::vector<TileName> candidate_tiles = Wang_query_for_tile_pos(tile_pos);

    if (candidate_tiles.size() == 0)
      dead_tiles.push_back(tile_pos);
    else {
      assert(candidate_tiles.size() ==
             1);  // tile is completed (4 edges are set) so at most 1 choice
      completed_tiles[tile_pos] = candidate_tiles[0];
    }

    uncompleted_tiles.erase(tile_pos);
  }
}

World::~World() {}