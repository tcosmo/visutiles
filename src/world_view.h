#pragma once

#include "global.h"
#include "world_controller.h"

class WorldView {
 public:
  WorldView(const WorldController& w_controller);
  ~WorldView();

 private:
  const WorldController& w_controller;
};