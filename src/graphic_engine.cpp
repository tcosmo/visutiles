#include "graphic_engine.h"

GraphicEngine::GraphicEngine(WorldController& world_controller,
                             WorldView& world_view, int screen_w, int screen_h)
    : world_controller(world_controller), world_view(world_view) {
  window.create(sf::VideoMode(screen_w, screen_h), visutiles_PROG_NAME);
  window.setFramerateLimit(TARGET_FPS);

  camera = window.getDefaultView();
  window.setView(camera);
  currentZoom = 1.0;
}

GraphicEngine::~GraphicEngine() {}

void GraphicEngine::run() {
  world_view.setPosition({500, 300});

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;
        }
      }

      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear(sf::Color(0.2 * 255, 0.2 * 255, 0.2 * 255));
    window.draw(world_view);
    window.display();
  }
}