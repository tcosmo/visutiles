#pragma once

#include "global.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "world.h"
#include "world_view.h"

class GraphicEngine {
 public:
  GraphicEngine(World& world, WorldView& world_view, const Tileset& tileset,
                int screen_w, int screen_h);
  ~GraphicEngine();

  void run();

 private:
  static bool is_ctrl_pressed();
  static bool is_shift_pressed();
  static bool is_alt_pressed();

  void print_simulation_report();

  World& world;
  WorldView& world_view;

  const Tileset& tileset;

  sf::RenderWindow window;

  void camera_init();
  void camera_center(const sf::Vector2f& where = {0, 0});
  void camera_translate(const sf::Vector2f& d_pos);
  void camera_zoom(float zoom_factor);
  void camera_reset();
  void handle_camera_events(const sf::Event& event);

  struct CameraParams {
    sf::View view;
    sf::View initial_view;
    sf::Vector2f default_trans_vec;
    float default_zoom_step;
    bool mouse_has_left;
    bool drag_move_mode;
    sf::Vector2i mouse_position;
  } camera_params;
};