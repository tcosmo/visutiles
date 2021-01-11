#pragma once

#include "global.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "world.h"

class GraphicEngine {
 public:
  GraphicEngine(World &world, int screen_w, int screen_h);
  ~GraphicEngine();

  void run();

 private:
  World &world;

  sf::RenderWindow window;

  sf::View camera;
  double currentZoom;
};