#pragma once

#include <map>

#include "global.h"

#include "tileset.h"

typedef std::map<sf::Vector2i, TileName, CompareTilesPositions> TileMap;
typedef std::map<OrderedPosCouple, EdgeColor, CompareOrderedPosCouple> EdgeMap;

struct EdgePosAndColor {
  OrderedPosCouple pos;
  EdgeColor color;

  EdgePosAndColor(const OrderedPosCouple& pos, const EdgeColor& color)
      : pos(pos), color(color) {}

  EdgePosAndColor(
      const std::pair<const OrderedPosCouple, EdgeColor>& pos_and_color) {
    pos = pos_and_color.first;
    color = pos_and_color.second;
  }
};

struct TilePosAndName {
  sf::Vector2i pos;
  TileName name;
};

// The world consists in an assembly (non necessarily connected) of tiles on
// the square grid. Each position can hold exactly one tile.
class World {
 public:
  World(Tileset& tileset);
  ~World();

  void set_edges(EdgeMap p_edges);
  const EdgeMap& get_edges() { return edges; }

  void update();

  size_t edge_count() const { return edges.size(); }

  size_t completed_tile_count() const { return completed_tiles.size(); }
  size_t uncompleted_tile_count() const { return uncompleted_tiles_pos.size(); }
  size_t dead_tile_count() const { return dead_tiles_pos.size(); }

  Tileset& tileset;  // not const because of query memoization

  void reset() {
    edges.clear();
    spawned_tiles_pos.clear();
    completed_tiles.clear();
    uncompleted_tiles_pos.clear();
    dead_tiles_pos.clear();
    clear_view_buffers();
    set_edges(input_edges);
  }

  /* To be used by view */
  const std::vector<EdgePosAndColor>& get_newly_added_edges() const {
    return newly_added_edges;
  }

  const std::vector<TilePosAndName>& get_newly_added_completed_tiles() const {
    return newly_completed_tiles;
  }

  const PosVec& get_newly_added_dead_tiles_pos() const {
    return newly_dead_tiles_pos;
  }

  const PosSet& get_newly_added_uncompleted_tiles_pos() const {
    return newly_uncompleted_tiles_pos;
  }

 private:
  EdgeMap edges;
  EdgeMap input_edges;

  // Contains the position of the tiles that were spawned so far.
  // A tile is spawned when at least one of its 4 edges is put into existence.
  PosSet spawned_tiles_pos;

  // Completed tiles are tiles with exactly 1 solution to their Wang colors
  TileMap completed_tiles;

  // Uncompleted tiles are tiles with at least 2 solutions to their Wang colors
  // This invariant is not always maintained as an update step might be
  // necessary in order to realise that a given tile is in fact dead or complete
  PosSet uncompleted_tiles_pos;
  // Dead tiles are tiles with no solution at all to their Wang colors
  PosSet dead_tiles_pos;

  std::array<EdgeColor, SQUARE_GRID_NEIGHBORING_SIZE>
  get_edges_colors_at_tile_pos(const sf::Vector2i& tile_pos);
  void add_edge_if_not_present(const EdgePosAndColor& edge);

  void spawn_tile_pos(const sf::Vector2i& tile_pos) {
    spawned_tiles_pos.insert(tile_pos);
    uncompleted_tiles_pos.insert(tile_pos);
    newly_uncompleted_tiles_pos.insert(tile_pos);
  };

  void print_edge(const EdgePosAndColor& edge);
  void print_all_edges();

  /* The following is for view exposure */
  std::vector<EdgePosAndColor>
      newly_added_edges;  // vector and not set because protected of redundancy
                          // by edges map
  std::vector<TilePosAndName>
      newly_completed_tiles;  // vector and not set because protected of
                              // redundancy as we fill it by iterating tiles by
                              // tiles

  PosVec newly_dead_tiles_pos;  // vector and not set because protected of
                                // redundancy as we fill it by iterating tiles
                                // by tiles

  PosSet newly_uncompleted_tiles_pos;  // set because possible redundancy at
                                       // insert time

  void clear_view_buffers();
};