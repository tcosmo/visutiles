#include "world.h"

World::World(Tileset& tileset) : tileset(tileset), missmatching_edge_count(0) {}

void World::print_edge(const EdgePosAndColor& edge) {
  printf("\t(%d,%d) (%d,%d) (%s,%d)\n", edge.pos.first.x, edge.pos.first.y,
         edge.pos.second.x, edge.pos.second.y, edge.color.first.c_str(),
         edge.color.second);
}

void World::print_all_edges() {
  for (const EdgePosAndColor& edge : edges) {
    print_edge(edge);
  }
}

void World::set_input_edges(EdgeMap p_edges) {
  input_edges = std::move(p_edges);
  for (const EdgePosAndColor& edge : input_edges) {
    add_edge_if_not_present(edge);
  }
}

std::array<EdgeColor, SQUARE_GRID_NEIGHBORING_SIZE>
World::get_edges_colors_at_tile_pos(const sf::Vector2i& tile_pos) {
  std::array<EdgeColor, SQUARE_GRID_NEIGHBORING_SIZE> to_return;

  std::array<OrderedPosCouple, 4> edges_pos =
      square_grid_edges_pos_from_tile_pos(tile_pos);

  for (size_t i_dir = 0; i_dir < SQUARE_GRID_NEIGHBORING_SIZE; i_dir += 1) {
    EdgeColor color = ANY_EDGE_COLOR;
    if (edges.find(edges_pos[i_dir]) != edges.end())
      color = edges[edges_pos[i_dir]];
    to_return[i_dir] = color;
  }

  return to_return;
}

void World::add_edge_if_not_present(const EdgePosAndColor& edge) {
  if (edges.find(edge.pos) != edges.end()) return;

  edges[edge.pos] = edge.color;
  newly_added_edges.push_back(edge);

  for (const sf::Vector2i& tile_pos :
       square_grid_tiles_pos_from_edge_pos(edge.pos)) {
    spawn_tile_pos(tile_pos);
  }
}

void World::clear_view_buffers() {
  newly_added_edges.clear();
  newly_completed_tiles.clear();
  newly_dead_tiles_pos.clear();
  newly_uncompleted_tiles_pos.clear();
}

// World update step: add edges of tiles that can be determined from their
// current constraints
void World::update() {
  clear_view_buffers();

  std::vector<sf::Vector2i> to_remove_from_uncompleted_tiles_pos;
  std::vector<EdgePosAndColor> edges_to_add;

  for (const sf::Vector2i& tile_pos : uncompleted_tiles_pos) {
    std::array<EdgeColor, SQUARE_GRID_NEIGHBORING_SIZE> tile_edges_colors =
        get_edges_colors_at_tile_pos(tile_pos);
    std::array<OrderedPosCouple, SQUARE_GRID_NEIGHBORING_SIZE> tile_edges_pos =
        square_grid_edges_pos_from_tile_pos(tile_pos);

    PossibleEdgesColorsAndTilesNames query_result =
        tileset.Wang_query(tile_edges_colors);

    // Add edges when they are the unique solution to Wang constraints
    for (size_t i_dir = 0; i_dir < SQUARE_GRID_NEIGHBORING_SIZE; i_dir += 1) {
      if (query_result.first[i_dir].size() == 1) {
        edges_to_add.push_back(EdgePosAndColor(
            {tile_edges_pos[i_dir], *query_result.first[i_dir].begin()}));
      }
    }

    // If only one tile is solution we record the tile as complete
    if (query_result.second.size() == 1) {
      completed_tiles[tile_pos] = query_result.second[0];
      to_remove_from_uncompleted_tiles_pos.push_back(tile_pos);
      newly_completed_tiles.push_back(
          TilePosAndName({tile_pos, query_result.second[0]}));
    }

    // If no tile is solution we record the tile as dead
    if (query_result.second.size() == 0) {
      dead_tiles_pos.insert(tile_pos);
      to_remove_from_uncompleted_tiles_pos.push_back(tile_pos);
      newly_dead_tiles_pos.push_back(tile_pos);
    }
  }

  // for edge mismatch tracking
  std::map<OrderedPosCouple, bool, CompareOrderedPosCouple> edge_pos_seen;

  for (const EdgePosAndColor& edge : edges_to_add) {
    if (edge_pos_seen.find(edge.pos) == edge_pos_seen.end()) {
      add_edge_if_not_present(edge);
    } else {
      // Check for mismatch
      if (edge.color != edges[edge.pos]) {
        set_mismatching_edge(edge.pos);
      }
    }

    edge_pos_seen[edge.pos] = true;
  }

  for (const sf::Vector2i& tile_pos : to_remove_from_uncompleted_tiles_pos) {
    uncompleted_tiles_pos.erase(tile_pos);
  }
}

// Set an edge to mismatch when a clash is detected
void World::set_mismatching_edge(const OrderedPosCouple& edge_pos) {
  assert(edges.find(edge_pos) != edges.end());

  auto added_edge_iterator =
      std::find(newly_added_edges.begin(), newly_added_edges.end(),
                EdgePosAndColor(edge_pos, edges[edge_pos]));

  assert(added_edge_iterator != newly_added_edges.end());
  newly_added_edges.erase(added_edge_iterator);

  missmatching_edge_count += 1;

  edges[edge_pos] = MISMATCH_EDGE_COLOR;
  newly_added_edges.push_back(EdgePosAndColor(edge_pos, MISMATCH_EDGE_COLOR));
}

World::~World() {}