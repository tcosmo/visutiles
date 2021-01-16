#pragma once

#include "global.h"
#include "world.h"

class WorldController {
 public:
  WorldController(World& w);
  ~WorldController();

  void init_world();

  std::vector<std::pair<sf::Vector2i, TileId>> newly_added_tiles;

 private:
  World& w;
};