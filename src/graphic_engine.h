#pragma once

#include "global.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "world_controller.h"
#include "world_view.h"

class GraphicEngine {
 public:
  GraphicEngine(WorldController& world_controller, WorldView& world_view,
                int screen_w, int screen_h);
  ~GraphicEngine();

  void run();

 private:
  WorldController& world_controller;
  WorldView& world_view;

  sf::RenderWindow window;

  sf::View camera;
  double currentZoom;
};