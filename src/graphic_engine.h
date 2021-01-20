#pragma once

#include "global.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "world_controller.h"
#include "world_view.h"

#include "camera.h"

class GraphicEngine {
 public:
  GraphicEngine(WorldController& world_controller, WorldView& world_view,
                const Tileset& tileset, int screen_w, int screen_h);
  ~GraphicEngine();

  void run();

 private:
  void print_simulation_report();

  WorldController& world_controller;
  WorldView& world_view;

  const Tileset& tileset;

  sf::RenderWindow window;

  Camera camera;
};