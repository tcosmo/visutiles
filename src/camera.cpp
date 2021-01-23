#include "graphic_engine.h"

void GraphicEngine::camera_init() {
  camera_params.default_trans_vec = {(float)GRAPHIC_TILE_SIZE,
                                     (float)GRAPHIC_TILE_SIZE};
  camera_params.view = window.getDefaultView();
  camera_center();
  camera_params.initial_view = sf::View(camera_params.view);
  window.setView(camera_params.view);
  camera_params.default_zoom_step = 1.5;
  camera_params.drag_move_mode = false;
  camera_params.mouse_has_left = false;
}

void GraphicEngine::camera_center(const sf::Vector2f& where) {
  camera_params.view.setCenter(where);
  window.setView(camera_params.view);
}

void GraphicEngine::camera_translate(const sf::Vector2f& d_pos) {
  camera_params.view.move(d_pos);
  window.setView(camera_params.view);
}

void GraphicEngine::camera_zoom(float zoom_factor) {
  camera_params.view.zoom(1 / zoom_factor);
  window.setView(camera_params.view);
}

void GraphicEngine::camera_reset() {
  camera_params.view = sf::View(camera_params.initial_view);
  window.setView(camera_params.view);
}

// Handling some standard camera UX (translation, zoom, drag move)
void GraphicEngine::handle_camera_events(const sf::Event& event) {
  // Translate view with Arrows and center when press C
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.code) {
      case sf::Keyboard::C:
        camera_center();
        break;

      case sf::Keyboard::R:
        camera_reset();
        break;

      case sf::Keyboard::Up:
        camera_translate(sf::Vector2f(VIEW_NORTH) *
                         camera_params.default_trans_vec);
        break;

      case sf::Keyboard::Down:
        camera_translate(sf::Vector2f(VIEW_SOUTH) *
                         camera_params.default_trans_vec);
        break;

      case sf::Keyboard::Right:
        camera_translate(sf::Vector2f(VIEW_EAST) *
                         camera_params.default_trans_vec);
        break;

      case sf::Keyboard::Left:
        camera_translate(sf::Vector2f(VIEW_WEST) *
                         camera_params.default_trans_vec);
        break;

      default:
        break;
    }
  }

  // Track mouse left events for placing mouse back in center if left
  if (event.type == sf::Event::MouseLeft) camera_params.mouse_has_left = true;
  if (event.type == sf::Event::MouseEntered)
    camera_params.mouse_has_left = false;

  // Zooming with CTRL + mouse wheel
  if (event.type == sf::Event::MouseWheelScrolled)
    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
      if (is_ctrl_pressed()) {
        float zoom_factor = camera_params.default_zoom_step;
        if (event.mouseWheelScroll.delta < 0) zoom_factor = 1 / zoom_factor;
        camera_zoom(zoom_factor);
      }
    }

  // Moving around by dragging the world with mouse wheel button
  if (event.type == sf::Event::MouseButtonPressed)
    if ((event.mouseButton.button == sf::Mouse::Middle)) {
      camera_params.drag_move_mode = true;
      camera_params.mouse_position = sf::Mouse::getPosition(window);
    }

  if (event.type == sf::Event::MouseMoved) {
    if (camera_params.drag_move_mode) {
      auto coordMouseMove =
          window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
      auto coordMouse = window.mapPixelToCoords(camera_params.mouse_position);
      camera_translate(coordMouseMove - coordMouse);
      camera_params.mouse_position = sf::Mouse::getPosition(window);
    }
  }

  if (event.type == sf::Event::MouseButtonReleased)
    if (event.mouseButton.button == sf::Mouse::Middle) {
      camera_params.drag_move_mode = false;

      if (camera_params.mouse_has_left)
        sf::Mouse::setPosition({static_cast<int>(window.getSize().x / 2),
                                static_cast<int>(window.getSize().y / 2)},
                               window);
    }
}