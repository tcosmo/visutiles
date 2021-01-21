#include "world_controller.h"

WorldController::WorldController(World& w) : w(w) {}

void WorldController::init_world() {}

void WorldController::init_world(TileMap initial_configuration) {
  w.tile_at_pos = std::move(initial_configuration);

  for (const std::pair<sf::Vector2i, TileId>& pos_and_tile : w.tile_at_pos) {
    newly_added_tiles.push_back(pos_and_tile);
  }
}

WorldController::~WorldController() {}