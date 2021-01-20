#pragma once

#include "global.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "world_controller.h"
#include "world_view.h"

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

  void camera_init();
  void camera_center(const sf::Vector2f& where = {0, 0});
  void camera_translate(const sf::Vector2f& d_pos);
  void camera_reset();
  void handle_camera_events(const sf::Event& event);

  sf::View camera_view;
  sf::View initial_camera_view;
  sf::Vector2f camera_translation_vec;
};