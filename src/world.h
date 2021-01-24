#pragma once

#include <map>

#include "global.h"

#include "tileset.h"

typedef std::map<sf::Vector2i, TileName, CompareTilesPositions> TileMap;
typedef std::map<OrderedPosCouple, EdgeColor, CompareOrderedPosCouple> EdgeMap;

typedef std::pair<OrderedPosCouple, EdgeColor> PosEdge;

// The world consists in an assembly (non necessarily connected) of tiles on
// the square grid. Each position can hold exactly one tile.
class World {
 public:
  World(Tileset& tileset);
  ~World();

  void set_edges(EdgeMap p_edges);
  const EdgeMap& get_edges() { return edges; }

  const std::vector<PosEdge>& get_newly_added_edges() const {
    return newly_added_edges;
  }

  void update();

  size_t edge_count() const { return edges.size(); }

  size_t completed_tile_count() const { return completed_tiles.size(); }
  size_t uncompleted_tile_count() const { return uncompleted_tiles.size(); }
  size_t dead_tile_count() const { return dead_tiles.size(); }
  size_t versa_tile_count() const { return versa_tiles.size(); }

  Tileset& tileset;  // not const because of query memoization

 private:
  EdgeMap edges;

  std::vector<PosEdge> newly_added_edges;

  TileMap completed_tiles;

  // Uncompleted tiles are tiles with < 4 edges and no solution yet
  // to their Wang colors
  PosSet uncompleted_tiles;
  // Dead tiles are tiles with no solution at all to their Wang colors
  PosVec dead_tiles;

  // Versa tiles are tiles with multiple solutions to their Wang colors
  // we store the number of solutions in the second elem of the pair
  std::vector<std::pair<sf::Vector2i, size_t>> versa_tiles;

  void remove_completed_and_dead_tiles();
  bool is_tile_completed(const sf::Vector2i& tile_pos);
  std::vector<TileName> Wang_query_for_tile_pos(const sf::Vector2i& tile_pos);
  void add_edges_for_tile(const sf::Vector2i& tile_pos, TileName tile_name);
  void add_tiles_for_newly_added_edges();

  void print_all_edges();
};