#include "graphic_engine.h"

void GraphicEngine::camera_init() {
  camera_translation_vec = {(float)tileset.tile_width,
                            (float)tileset.tile_height};
  camera_view = window.getDefaultView();
  initial_camera_view = camera_view;
  camera_center();
  window.setView(camera_view);
  camera_zoom_step = 1.5;
}

void GraphicEngine::camera_center(const sf::Vector2f& where) {
  camera_view.setCenter(where);
  window.setView(camera_view);
}

void GraphicEngine::camera_translate(const sf::Vector2f& d_pos) {
  camera_view.move(d_pos);
  window.setView(camera_view);
}

void GraphicEngine::camera_zoom(float zoom_factor) {
  camera_view.zoom(1 / zoom_factor);
  window.setView(camera_view);
}

void GraphicEngine::camera_reset() { camera_view = initial_camera_view; }

void GraphicEngine::handle_camera_events(const sf::Event& event) {
  // Translate view with Arrows and center when press C
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.code) {
      case sf::Keyboard::C:
        camera_center();
        break;

      case sf::Keyboard::Up:
        camera_translate(sf::Vector2f(VIEW_NORTH) * camera_translation_vec);
        break;

      case sf::Keyboard::Down:
        camera_translate(sf::Vector2f(VIEW_SOUTH) * camera_translation_vec);
        break;

      case sf::Keyboard::Right:
        camera_translate(sf::Vector2f(VIEW_EAST) * camera_translation_vec);
        break;

      case sf::Keyboard::Left:
        camera_translate(sf::Vector2f(VIEW_WEST) * camera_translation_vec);
        break;

      default:
        break;
    }
  }

  // // Track mouse left events for replacing mouse in center if left
  // if (event.type == sf::Event::MouseLeft) cameraMouseLeft = true;
  // if (event.type == sf::Event::MouseEntered) cameraMouseLeft = false;

  if (event.type == sf::Event::MouseWheelScrolled)
    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
      if (is_ctrl_pressed()) {
        float zoom_factor = camera_zoom_step;
        if (event.mouseWheelScroll.delta < 0) zoom_factor = 1 / zoom_factor;
        camera_zoom(zoom_factor);
      }
    }

  // if (event.type == sf::Event::MouseButtonPressed)
  //   if ((event.mouseButton.button == sf::Mouse::Middle)) {
  //     moveCameraMode = true;
  //     cameraMousePosition = sf::Mouse::getPosition(window);
  //   }

  // if (event.type == sf::Event::MouseMoved) {
  //   if (moveCameraMode) {
  //     auto coordMouseMove =
  //         window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
  //     auto coordMouse = window.mapPixelToCoords(cameraMousePosition);
  //     cameraTranslate(coordMouse - coordMouseMove);
  //     cameraMousePosition = sf::Mouse::getPosition(window);
  //   }
  // }

  // if (event.type == sf::Event::MouseButtonReleased)
  //   if (event.mouseButton.button == sf::Mouse::Middle) {
  //     moveCameraMode = false;

  //     if (cameraMouseLeft)
  //       sf::Mouse::setPosition({static_cast<int>(window.getSize().x / 2),
  //                               static_cast<int>(window.getSize().y / 2)},
  //                              window);
  //   }
}