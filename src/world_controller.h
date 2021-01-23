#pragma once

#include "global.h"
#include "world.h"

class WorldController {
 public:
  WorldController(World& w, Tileset& tileset);
  ~WorldController();

  void init_world();
  void init_world(EdgeMap initial_configuration);

  const std::vector<PosEdge>& get_newly_added_edges() const {
    return newly_added_edges;
  };

  size_t get_edge_count() { return w.edges.size(); }

 private:
  World& w;
  std::vector<PosEdge> newly_added_edges;
  Tileset& tileset;  // cannot be const because of query memoization
};