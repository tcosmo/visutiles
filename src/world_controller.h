#pragma once

#include "global.h"
#include "world.h"

class WorldController {
 public:
  WorldController(World& w);
  ~WorldController();

 private:
  World& w;
};