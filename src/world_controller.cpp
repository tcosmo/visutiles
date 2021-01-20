#include "world_controller.h"

WorldController::WorldController(World& w) : w(w) {}

void WorldController::init_world() {
  newly_added_tiles.clear();
  newly_added_tiles.push_back({{0, 0}, 0});
  newly_added_tiles.push_back({{0, 1}, 1});
  newly_added_tiles.push_back({{1, 0}, 2});
  newly_added_tiles.push_back({{1, 1}, 3});
  newly_added_tiles.push_back({{-1, 0}, 4});
  newly_added_tiles.push_back({{-2, 0}, 5});

  for (int i = 0; i < 20000; i += 1) {
    newly_added_tiles.push_back({{2, i}, 5});
  }

  for (const std::pair<sf::Vector2i, TileId>& pos_and_tile :
       newly_added_tiles) {
    w.tile_at_pos[pos_and_tile.first] = pos_and_tile.second;
  }
}

WorldController::~WorldController() {}