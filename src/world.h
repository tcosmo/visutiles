#pragma once

#include <map>

#include "global.h"

#include "tileset.h"

typedef std::map<OrderedPosCouple, EdgeColor, CompareOrderedPosCouple> EdgeMap;

typedef std::pair<OrderedPosCouple, EdgeColor> PosEdge;

// The world consists in an assembly (non necessarily connected) of tiles on
// the square grid. Each position can hold exactly one tile.
class World {
 public:
  World(const Tileset& tileset);
  ~World();

  /* Attributes */
  EdgeMap edges;

  const Tileset& tileset;

 private:
};