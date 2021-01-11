#include "world.h"

World::World(bool allow_non_deterministic_attachments, GrowthMode growth_mode)
    : allow_non_deterministic_attachments(allow_non_deterministic_attachments),
      growth_mode(growth_mode)

{}

PosVec World::get_neighbouring_tiles_pos(const sf::Vector2i& pos) {
  // TODO: make this function more generic to handle other types of grid
  PosVec to_return;
  for (const sf::Vector2i& dir : SQUARE_GRID_DIR) {
    if (tiles.find(dir) != tiles.end()) {
      to_return.push_back(dir);
    }
  }
  return to_return;
}

void World::grow() { newest_tiles_pos.clear(); }

World::~World() {}