#pragma once

#include "global.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Camera {
 public:
  Camera();
  ~Camera();

  void set_translation_vector(sf::Vector2f p_translation_vector) {
    translation_vector = p_translation_vector;
  };

  void init_view(const sf::RenderWindow& window);

  void center(const sf::Vector2f& where = {0, 0});
  void translate(const sf::Vector2f& d_pos);
  void handle_camera_events(const sf::Event& event);
  void reset();

  bool has_changed;
  sf::View camera_view;

 private:
  sf::View initial_view;
  sf::Vector2f translation_vector;
};
