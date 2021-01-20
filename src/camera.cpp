#include "camera.h"

Camera::Camera() : has_changed(false) {}

void Camera::init_view(const sf::RenderWindow& window) {
  camera_view = window.getDefaultView();
  center();
  initial_view = camera_view;
  has_changed = true;
}

void Camera::center(const sf::Vector2f& where) {
  camera_view.setCenter(where);
  has_changed = true;
}

void Camera::translate(const sf::Vector2f& d_pos) {
  camera_view.move({d_pos.x, d_pos.y * -1});
  has_changed = true;
}

void Camera::reset() {
  camera_view = initial_view;
  has_changed = true;
}

void Camera::handle_camera_events(const sf::Event& event) {
  // Translate view with Arrows and center when press C if (event.type ==
  // sf::Event::KeyPressed) {
  switch (event.key.code) {
    case sf::Keyboard::C:
      center();
      break;

    case sf::Keyboard::Up:
      translate(sf::Vector2f({0, 1}) * translation_vector);
      break;

    case sf::Keyboard::Down:
      translate(sf::Vector2f({0, -1}) * translation_vector);
      break;

    case sf::Keyboard::Right:
      translate(sf::Vector2f({1, 0}) * translation_vector);
      break;

    case sf::Keyboard::Left:
      translate(sf::Vector2f({-1, 0}) * translation_vector);
      break;

    default:
      break;
  }

  // // Track mouse left events for replacing mouse in center if left
  // if (event.type == sf::Event::MouseLeft) cameraMouseLeft = true;
  // if (event.type == sf::Event::MouseEntered) cameraMouseLeft = false;

  // if (event.type == sf::Event::MouseWheelScrolled)
  //   if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
  //     if (isControlPressed()) {
  //       float zoomFactor = DEFAULT_CAM_ZOOM_STEP;
  //       if (event.mouseWheelScroll.delta < 0) zoomFactor = 1 / zoomFactor;
  //       cameraZoom(zoomFactor);
  //     }
  //   }

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

Camera::~Camera() {}