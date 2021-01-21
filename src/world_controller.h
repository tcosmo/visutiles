#pragma once

#include "global.h"
#include "world.h"

class WorldController {
 public:
  WorldController(World& w);
  ~WorldController();

  void init_world();
  void init_world(TileMap initial_configuration);

  const std::vector<std::pair<sf::Vector2i, TileId>>& get_newly_added_tiles()
      const {
    return newly_added_tiles;
  };

  size_t get_tile_count() { return w.tile_at_pos.size(); }

 private:
  World& w;
  std::vector<std::pair<sf::Vector2i, TileId>> newly_added_tiles;
};