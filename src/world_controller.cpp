#include "world_controller.h"

WorldController::WorldController(World& w, Tileset& tileset)
    : w(w), tileset(tileset) {}

void WorldController::init_world() {}

void WorldController::init_world(EdgeMap initial_configuration) {
  w.edges = std::move(initial_configuration);

  for (const PosEdge& pos_and_edge : w.edges) {
    newly_added_edges.push_back(pos_and_edge);
  }
}

WorldController::~WorldController() {}